/*
  Copyright (C) 2024 Bertie Cartwright

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "client/newClientWorld.h"

#include <thread>
#include <cmath>
#include <time.h>
#include <iostream>
#include <random>
#include <algorithm>

#include "client/meshBuilder.h"
#include "core/constants.h"
#include "core/random.h"
#include "core/serverWorld.h"
#include "core/terrainGen.h"

namespace client {

static bool chunkMeshUploaded[8] = { false, false, false, false, false, false, false, false };
static bool relableCompleted = false;

NewClientWorld::NewClientWorld(unsigned short renderDistance, unsigned long long seed, bool singleplayer, ENetPeer* peer, ENetHost* client)
    : integratedServer(singleplayer, seed) {
    //seed the random number generator and the simplex noise
    m_seed = seed;
    PCG_SeedRandom32(m_seed);
    seedNoise();

    m_renderDistance = renderDistance + 1;
    m_renderDiameter = m_renderDistance * 2 + 1;
    m_meshedChunksDistance = 0.0f;
    m_fogDistance = 0.0f;
    m_timeByDTs = 0.0;
    m_mouseCalls = 0;
    m_renderingFrame = false;
    m_renderThreadWaitingForArrIndicesVectors = false;

    int playerPosition[3] = { 0, 0, 0 };
    int chunkCoords[3];
    float minUnloadedChunkDistance = ((m_renderDistance + 1) * (m_renderDistance + 1));

    m_emptyIndexBuffer = new IndexBuffer();
    m_emptyVertexBuffer = new VertexBuffer();
    m_emptyVertexArray = new VertexArray(true);

    //allocate arrays on the heap for the mesh to be built
    //do this now so that the same array can be reused for each chunk
    m_numChunkLoadingThreads = std::max(1u, std::min(8u, std::thread::hardware_concurrency() - 1));
    if (!m_numChunkLoadingThreads) {
        m_numChunkLoadingThreads = 1;
    }
    else if (m_numChunkLoadingThreads > 4) {
        m_numChunkLoadingThreads = 4;
    }
    m_numChunkVertices = new unsigned int[m_numChunkLoadingThreads];
    m_numChunkIndices = new unsigned int[m_numChunkLoadingThreads];
    m_numChunkWaterVertices = new unsigned int[m_numChunkLoadingThreads];
    m_numChunkWaterIndices = new unsigned int[m_numChunkLoadingThreads];
    m_chunkVertices = new float*[m_numChunkLoadingThreads];
    m_chunkIndices = new unsigned int*[m_numChunkLoadingThreads];
    m_chunkWaterVertices = new float*[m_numChunkLoadingThreads];
    m_chunkWaterIndices = new unsigned int*[m_numChunkLoadingThreads];
    m_chunkPosition = new Position[m_numChunkLoadingThreads];
    m_chunkMeshReady = new bool[m_numChunkLoadingThreads];
    m_chunkMeshReadyCV = new std::condition_variable[m_numChunkLoadingThreads];
    m_chunkMeshReadyMtx = new std::mutex[m_numChunkLoadingThreads];
    m_threadWaiting = new bool[m_numChunkLoadingThreads];
    m_relableNeeded = true;
    
    for (int i = 0; i < m_numChunkLoadingThreads; i++) {
        m_chunkVertices[i] = new float[4 * 12 * 6 * constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
        m_chunkIndices[i] = new unsigned int[4 * 18 * constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
        m_chunkWaterVertices[i] = new float[4 * 12 * 6 * constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
        m_chunkWaterIndices[i] = new unsigned int[4 * 18 * constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
        m_chunkMeshReady[i] = false;
        m_threadWaiting[i] = false;
    }
    m_singleplayer = singleplayer;
    if (singleplayer) {
        int playerBlockPosition[3] = { 0, 0, 0 };
        float playerSubBlockPosition[3] = { 0.0f, 0.0f, 0.0f };
        integratedServer.addPlayer(playerBlockPosition, playerSubBlockPosition, m_renderDistance);
    }

    //calculate the offset chunk numbers for the neighbouring chunks
    m_neighbouringChunkOffets[0] = { 0, -1, 0 };
    m_neighbouringChunkOffets[1] = { 0, 0, -1 };
    m_neighbouringChunkOffets[2] = { -1, 0, 0 };
    m_neighbouringChunkOffets[3] = { 1, 0, 0 };
    m_neighbouringChunkOffets[4] = { 0, 0, 1 };
    m_neighbouringChunkOffets[5] = { 0, 1, 0 };

    int i = 0;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {
                m_neighbouringChunkIncludingDiaganalOffsets[i] = { x, y, z };
                i++;
            }
        }
    }
}

void NewClientWorld::renderChunks(Renderer mainRenderer, Shader& blockShader, Shader& waterShader, glm::mat4 viewMatrix, glm::mat4 projMatrix, int* playerBlockPosition, float aspectRatio, float fov, double DT) {
    if (m_chunkIndexBuffers.size() != m_meshedChunks.size()) {
        std::cout << "bad\n";
    }
    //else {
    //    cout << "good\n";
    //}
    Frustum viewFrustum = m_viewCamera->createViewFrustum(aspectRatio, fov, 0, 20);
    m_renderingFrame = true;
    float chunkCoordinates[3];
    //float centredChunkCoordinates[3];
    //render blocks
    blockShader.bind();
    blockShader.setUniformMat4f("u_proj", projMatrix);
    m_timeByDTs += DT;
    while (m_timeByDTs > (1.0/(double)constants::visualTPS)) {
        constexpr double fac = 0.016;
        m_fogDistance = m_fogDistance * (1.0 - fac) + (sqrt(m_meshedChunksDistance) - 2.0) * fac * constants::CHUNK_SIZE;
        m_timeByDTs -= (1.0/(double)constants::visualTPS);
    }
    blockShader.setUniform1f("u_renderDistance", m_fogDistance);
    if (m_meshUpdates.size() > 0) {
        auto tp1 = std::chrono::high_resolution_clock::now();
        while (m_meshUpdates.size() > 0) {
            std::cout << "m_numMeshUpdates > 0: " << m_meshUpdates.size() << "\n";
            doRenderThreadJobs();
        }
        auto tp2 = std::chrono::high_resolution_clock::now();
        std::cout << "waited " << std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() << "us for chunks to remesh\n";
    }
    for (const auto& [chunkPosition, ib] : m_chunkIndexBuffers) {
        if (m_chunkIndexBuffers.at(chunkPosition)->getCount() > 0) {
            chunkCoordinates[0] = chunkPosition.x * constants::CHUNK_SIZE - playerBlockPosition[0];
            chunkCoordinates[1] = chunkPosition.y * constants::CHUNK_SIZE - playerBlockPosition[1];
            chunkCoordinates[2] = chunkPosition.z * constants::CHUNK_SIZE - playerBlockPosition[2];
            AABB aabb(glm::vec3(chunkCoordinates[0], chunkCoordinates[1], chunkCoordinates[2]), glm::vec3(chunkCoordinates[0] + constants::CHUNK_SIZE, chunkCoordinates[1] + constants::CHUNK_SIZE, chunkCoordinates[2] + constants::CHUNK_SIZE));
            if (aabb.isOnFrustum(viewFrustum)) {
                glm::mat4 modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(chunkCoordinates[0], chunkCoordinates[1], chunkCoordinates[2]));
                //update the MVP uniform
                blockShader.setUniformMat4f("u_modelView", viewMatrix * modelMatrix);
                m_chunkVertexArrays.at(chunkPosition)->bind();
                mainRenderer.draw(*m_chunkVertexArrays.at(chunkPosition), *m_chunkIndexBuffers.at(chunkPosition), blockShader);
                doRenderThreadJobs();
            }
        }
    }
    if (m_relableNeeded) {
        relableCompleted = false;
    }
    auto tp2 = std::chrono::high_resolution_clock::now();
    //render water
    waterShader.bind();
    waterShader.setUniformMat4f("u_proj", projMatrix);
    waterShader.setUniform1f("u_renderDistance", m_fogDistance);
    for (const auto& [chunkPosition, ib] : m_chunkWaterIndexBuffers) {
        std::cout << chunkPosition.x << ", " << chunkPosition.y << ", " << chunkPosition.z << "\n";
        if (m_chunkWaterIndexBuffers.at(chunkPosition)->getCount() > 0) {
            chunkCoordinates[0] = chunkPosition.x * constants::CHUNK_SIZE - playerBlockPosition[0];
            chunkCoordinates[1] = chunkPosition.y * constants::CHUNK_SIZE - playerBlockPosition[1];
            chunkCoordinates[2] = chunkPosition.z * constants::CHUNK_SIZE - playerBlockPosition[2];
            AABB aabb(glm::vec3(chunkCoordinates[0], chunkCoordinates[1], chunkCoordinates[2]), glm::vec3(chunkCoordinates[0] + constants::CHUNK_SIZE, chunkCoordinates[1] + constants::CHUNK_SIZE, chunkCoordinates[2] + constants::CHUNK_SIZE));
            if (aabb.isOnFrustum(viewFrustum)) {
                glm::mat4 modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(chunkCoordinates[0], chunkCoordinates[1], chunkCoordinates[2]));
                //update the MVP uniform
                waterShader.setUniformMat4f("u_modelView", viewMatrix * modelMatrix);
                m_chunkWaterVertexArrays.at(chunkPosition)->bind();
                mainRenderer.draw(*m_chunkWaterVertexArrays.at(chunkPosition), *m_chunkWaterIndexBuffers.at(chunkPosition), waterShader);
                doRenderThreadJobs();
            }
        }
    }
    m_renderingFrame = false;
}

void NewClientWorld::doRenderThreadJobs() {
    relableChunksIfNeeded();
    for (char threadNum = 0; threadNum < m_numChunkLoadingThreads; threadNum++) {
        if (m_chunkMeshReady[threadNum]) {
            uploadChunkMesh(threadNum);
            // lock release 
            std::lock_guard<std::mutex> lock(m_chunkMeshReadyMtx[threadNum]);
            chunkMeshUploaded[threadNum] = true;
            m_chunkMeshReady[threadNum] = false;
            // notify consumer when done 
            m_chunkMeshReadyCV[threadNum].notify_one();
        }
    }
    //process the mouse input occasionally
    m_mouseCalls += 1;
    if (m_mouseCalls > 100) {
        processMouseInput();
        m_mouseCalls = 0;
    }
}

void NewClientWorld::updatePlayerPos(float playerX, float playerY, float playerZ) {
    m_newPlayerChunkPosition[0] = -1 * (playerX < 0) + playerX / constants::CHUNK_SIZE;
    m_newPlayerChunkPosition[1] = -1 * (playerY < 0) + playerY / constants::CHUNK_SIZE;
    m_newPlayerChunkPosition[2] = -1 * (playerZ < 0) + playerZ / constants::CHUNK_SIZE;
}

void NewClientWorld::relableChunksIfNeeded() {
    if (!m_relableNeeded) {
        bool relableNeeded = !((m_playerChunkPosition[0] == m_newPlayerChunkPosition[0])
            && (m_playerChunkPosition[1] == m_newPlayerChunkPosition[1])
            && (m_playerChunkPosition[2] == m_newPlayerChunkPosition[2]));
        m_relableNeeded = relableNeeded;
        relableCompleted = !m_relableNeeded;
        for (char i = 0; i < 3; i++) {
            m_updatingPlayerChunkPosition[i] = m_newPlayerChunkPosition[i];
        }
        int blockPosition[3] = { m_playerChunkPosition[0] * constants::CHUNK_SIZE,
                                  m_playerChunkPosition[1] * constants::CHUNK_SIZE,
                                  m_playerChunkPosition[2] * constants::CHUNK_SIZE };
        float subBlockPosition[3] = { 0.0f, 0.0f, 0.0f };
        integratedServer.updatePlayerPos(0, blockPosition, subBlockPosition);
    }
    //if the player has moved chunk, update the list of loaded chunks
    if (m_relableNeeded && (!m_renderingFrame)) {
        //wait for all the mesh builder threads to finish their jobs
        bool readyToRelable = false;
        for (char threadNum = 0; threadNum < m_numChunkLoadingThreads; threadNum++) {
            readyToRelable |= !m_threadWaiting[threadNum];
        }
        readyToRelable = !readyToRelable;
        if (!readyToRelable) {
            return;
        }

        unloadAndRelableChunks();
    }
}

unsigned int NewClientWorld::getChunkNumber(int* chunkCoords, int* playerChunkCoords) {
    int adjustedChunkCoords[3];
    for (unsigned char i = 0; i < 3; i++) {
        adjustedChunkCoords[i] = chunkCoords[i] - playerChunkCoords[i] + m_renderDistance;
    }
    
    unsigned int chunkNumber = adjustedChunkCoords[1] * m_renderDiameter * m_renderDiameter;
    chunkNumber += adjustedChunkCoords[2] * m_renderDiameter;
    chunkNumber += adjustedChunkCoords[0];

    return chunkNumber;
}

void NewClientWorld::getChunkCoords(int* chunkCoords, unsigned int chunkNumber, int* playerChunkCoords) {
    int adjustedChunkCoords[3];
    adjustedChunkCoords[0] = chunkNumber % m_renderDiameter;
    adjustedChunkCoords[1] = chunkNumber / (m_renderDiameter * m_renderDiameter);
    adjustedChunkCoords[2] = (chunkNumber - adjustedChunkCoords[1] * m_renderDiameter * m_renderDiameter) / m_renderDiameter;

    for (unsigned char i = 0; i < 3; i++) {
        chunkCoords[i] = adjustedChunkCoords[i] - m_renderDistance + playerChunkCoords[i];
    }
}

void NewClientWorld::loadChunksAroundPlayer(char threadNum) {
    integratedServer.findChunksToLoad();
    Position chunkPosition;
    if (integratedServer.loadChunk(&chunkPosition)) {
        m_unmeshedChunksMtx.lock();
        m_unmeshedChunks.insert(chunkPosition);
        m_unmeshedChunksMtx.unlock();
    }
    if (m_relableNeeded && (m_meshUpdates.size() == 0)) {
        m_threadWaiting[threadNum] = true;
        // locking 
        std::unique_lock<std::mutex> lock(m_relableNeededMtx);
        // waiting 
        m_relableNeededCV.wait(lock, [] { return relableCompleted; });
        m_threadWaiting[threadNum] = false;
    }
    buildMeshesForNewChunksWithNeighbours(threadNum);
}

void NewClientWorld::unloadAndRelableChunks() {
    //unload any meshes and chunks that are out of render distance
    float distance = 0;
    Position lastChunkPosition;
    for (auto& chunkPosition : m_meshedChunks) {
        if (distance >= ((m_renderDistance - 0.001f) * (m_renderDistance - 0.001f))) {
            unloadMesh(lastChunkPosition);
            std::cout << "unload\n";
        }
        distance = 0;
        distance += (chunkPosition.x - m_updatingPlayerChunkPosition[0]) * (chunkPosition.x - m_updatingPlayerChunkPosition[0]);
        distance += (chunkPosition.y - m_updatingPlayerChunkPosition[1]) * (chunkPosition.y - m_updatingPlayerChunkPosition[1]);
        distance += (chunkPosition.z - m_updatingPlayerChunkPosition[2]) * (chunkPosition.z - m_updatingPlayerChunkPosition[2]);
        lastChunkPosition = chunkPosition;
    }

    //update the player's chunk position
    m_playerChunkPosition[0] = m_updatingPlayerChunkPosition[0];
    m_playerChunkPosition[1] = m_updatingPlayerChunkPosition[1];
    m_playerChunkPosition[2] = m_updatingPlayerChunkPosition[2];

    relableCompleted = true;
    m_relableNeeded = false;
    // lock release 
    std::lock_guard<std::mutex> lock(m_relableNeededMtx);
    // notify consumer when done
    m_relableNeededCV.notify_all();
}

bool NewClientWorld::chunkHasNeighbours(const Position& chunkPosition) {
    if ((abs(chunkPosition.x - m_playerChunkPosition[0]) == m_renderDistance)
        || (abs(chunkPosition.y - m_playerChunkPosition[1]) == m_renderDistance)
        || (abs(chunkPosition.z - m_playerChunkPosition[2]) == m_renderDistance)) {
        return false;
    }

    for (unsigned char i = 0; i < 27; i++) {
        if (!(integratedServer.chunkLoaded(chunkPosition + m_neighbouringChunkIncludingDiaganalOffsets[i]))) {
            return false;
        }
    }

    return true;
}

void NewClientWorld::unloadMesh(const Position& chunkPosition) {
    //m_unmeshedChunkArrayIndices.push_back(m_meshedChunkPositions[chunkVectorIndex]);

    m_meshedChunks.erase(chunkPosition);
    if (m_chunkIndexBuffers.at(chunkPosition)->getCount() > 0) {
        delete m_chunkVertexArrays.at(chunkPosition);
        delete m_chunkVertexBuffers.at(chunkPosition);
        delete m_chunkIndexBuffers.at(chunkPosition);
    }

    m_chunkVertexArrays.erase(chunkPosition);
    m_chunkVertexBuffers.erase(chunkPosition);
    m_chunkIndexBuffers.erase(chunkPosition);

    if (m_chunkWaterIndexBuffers.at(chunkPosition)->getCount() > 0) {
        delete m_chunkWaterVertexArrays.at(chunkPosition);
        delete m_chunkWaterVertexBuffers.at(chunkPosition);
        delete m_chunkWaterIndexBuffers.at(chunkPosition);
    }

    m_chunkWaterVertexArrays.erase(chunkPosition);
    m_chunkWaterVertexBuffers.erase(chunkPosition);
    m_chunkWaterIndexBuffers.erase(chunkPosition);

    m_unmeshedChunksMtx.lock();
    m_unmeshedChunks.insert(chunkPosition);
    m_unmeshedChunksMtx.unlock();
}

void NewClientWorld::addChunkMesh(const Position& chunkPosition, char threadNum) {
    int chunkCoords[3] = { chunkPosition.x, chunkPosition.y, chunkPosition.z };

    //set up containers for data for buffers
    m_numChunkVertices[threadNum] = 0;
    m_numChunkIndices[threadNum] = 0;
    m_numChunkWaterVertices[threadNum] = 0;
    m_numChunkWaterIndices[threadNum] = 0;

    //get the chunk number
    unsigned int chunkNumber = getChunkNumber(chunkCoords, m_playerChunkPosition);

    //generate the mesh
    MeshBuilder(integratedServer.getChunk(chunkPosition)).buildMesh(m_chunkVertices[threadNum], &m_numChunkVertices[threadNum], m_chunkIndices[threadNum], &m_numChunkIndices[threadNum], m_chunkWaterVertices[threadNum], &m_numChunkWaterVertices[threadNum], m_chunkWaterIndices[threadNum], &m_numChunkWaterIndices[threadNum]);

    //if the chunk is empty fill the data with empty values to save interrupting the render thread
    if ((m_numChunkIndices[threadNum] == 0) && (m_numChunkWaterIndices[threadNum] == 0)) {
        m_accessingArrIndicesVectorsMtx.lock();
        while (m_renderThreadWaitingForArrIndicesVectors) {
            m_accessingArrIndicesVectorsMtx.unlock();
            m_renderThreadWaitingForArrIndicesVectorsMtx.lock();
            m_accessingArrIndicesVectorsMtx.lock();
            m_renderThreadWaitingForArrIndicesVectorsMtx.unlock();
        }
        m_chunkVertexArrays[chunkPosition] = m_emptyVertexArray;
        m_chunkVertexBuffers[chunkPosition] = m_emptyVertexBuffer;
        m_chunkIndexBuffers[chunkPosition] = m_emptyIndexBuffer;
        m_chunkWaterVertexArrays[chunkPosition] = m_emptyVertexArray;
        m_chunkWaterVertexBuffers[chunkPosition] = m_emptyVertexBuffer;
        m_chunkWaterIndexBuffers[chunkPosition] = m_emptyIndexBuffer;
        m_meshedChunks.insert(chunkPosition);
        auto it = m_meshUpdates.find(chunkPosition);
        if (it != m_meshUpdates.end()) {
            m_meshUpdates.erase(it);
        }

        m_accessingArrIndicesVectorsMtx.unlock();

        return;
    }
    //wait for the render thread to upload the mesh to the GPU
    m_chunkPosition[threadNum] = chunkPosition;
    m_chunkMeshReady[threadNum] = true;
    chunkMeshUploaded[threadNum] = false;

    // locking
    std::unique_lock<std::mutex> lock(m_chunkMeshReadyMtx[threadNum]);
    // waiting
    while (!chunkMeshUploaded[threadNum]) {
        m_chunkMeshReadyCV[threadNum].wait(lock);
    }
}

void NewClientWorld::uploadChunkMesh(char threadNum) {
    //TODO: precalculate this VBL object
    //create vertex buffer layout
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(1);

    VertexArray *va, *waterVa;
    VertexBuffer *vb, *waterVb;
    IndexBuffer *ib, *waterIb;

    if (m_numChunkIndices[threadNum] > 0) {
        //create vertex array
        va = new VertexArray;

        //create vertex buffer
        vb = new VertexBuffer(m_chunkVertices[threadNum], m_numChunkVertices[threadNum] * sizeof(float));

        va->addBuffer(*vb, layout);

        //create index buffer
        ib = new IndexBuffer(m_chunkIndices[threadNum], m_numChunkIndices[threadNum]);
    }
    else {
        va = m_emptyVertexArray;
        vb = m_emptyVertexBuffer;
        ib = m_emptyIndexBuffer;
    }

    if (m_numChunkWaterIndices[threadNum] > 0) {
        //create vertex array
        waterVa = new VertexArray;

        //create vertex buffer
        waterVb = new VertexBuffer(m_chunkWaterVertices[threadNum], m_numChunkWaterVertices[threadNum] * sizeof(float));

        waterVa->addBuffer(*waterVb, layout);

        //create index buffer
        waterIb = new IndexBuffer(m_chunkWaterIndices[threadNum], m_numChunkWaterIndices[threadNum]);
    }
    else {
        waterVa = m_emptyVertexArray;
        waterVb = m_emptyVertexBuffer;
        waterIb = m_emptyIndexBuffer;
    }

    m_renderThreadWaitingForArrIndicesVectorsMtx.lock();
    m_renderThreadWaitingForArrIndicesVectors = true;
    m_accessingArrIndicesVectorsMtx.lock();
    m_renderThreadWaitingForArrIndicesVectors = false;
    m_renderThreadWaitingForArrIndicesVectorsMtx.unlock();
    m_chunkVertexArrays[m_chunkPosition[threadNum]] = va;
    m_chunkVertexBuffers[m_chunkPosition[threadNum]] = vb;
    m_chunkIndexBuffers[m_chunkPosition[threadNum]] = ib;
    m_chunkWaterVertexArrays[m_chunkPosition[threadNum]] = waterVa;
    m_chunkWaterVertexBuffers[m_chunkPosition[threadNum]] = waterVb;
    m_chunkWaterIndexBuffers[m_chunkPosition[threadNum]] = waterIb;
    m_meshedChunks.insert(m_chunkPosition[threadNum]);
    auto it = m_meshUpdates.find(m_chunkPosition[threadNum]);
    if (it != m_meshUpdates.end()) {
        m_meshUpdates.erase(it);
    }
    m_accessingArrIndicesVectorsMtx.unlock();
}

void NewClientWorld::buildMeshesForNewChunksWithNeighbours(char threadNum) {
    m_unmeshedChunksMtx.lock();
    for (const auto chunkPosition : m_unmeshedChunks) {
        if (chunkHasNeighbours(chunkPosition)) {
            m_unmeshedChunks.erase(chunkPosition);
            m_unmeshedChunksMtx.unlock();
            addChunkMesh(chunkPosition, threadNum);
            break;
        }
    }
    m_unmeshedChunksMtx.unlock();
}

unsigned char NewClientWorld::shootRay(glm::vec3 startSubBlockPos, int* startBlockPosition, glm::vec3 direction, int* breakBlockCoords, int* placeBlockCoords) {
    //TODO: improve this to make it need less steps
    glm::vec3 rayPos = startSubBlockPos;
    int blockPos[3];
    int steps = 0;
    while (steps < 180) {
        rayPos += direction * 0.025f;
        for (unsigned char ii = 0; ii < 3; ii++) {
            blockPos[ii] = floor(rayPos[ii]) + startBlockPosition[ii];
        }
        short blockType = getBlock(blockPos);
        if ((blockType != 0) && (blockType != 4)) {
            for (unsigned char ii = 0; ii < 3; ii++) {
                breakBlockCoords[ii] = blockPos[ii];
            }

            rayPos -= direction * 0.025f;

            for (unsigned char ii = 0; ii < 3; ii++) {
                placeBlockCoords[ii] = floor(rayPos[ii]) + startBlockPosition[ii];
            }
            return 2;
        }
        steps++;
    }
    return 0;
}

void NewClientWorld::replaceBlock(int* blockCoords, unsigned char blockType) {
    Position chunkPosition;
    chunkPosition.x = -1 * (blockCoords[0] < 0) + blockCoords[0] / constants::CHUNK_SIZE;
    chunkPosition.y = -1 * (blockCoords[1] < 0) + blockCoords[1] / constants::CHUNK_SIZE;
    chunkPosition.z = -1 * (blockCoords[2] < 0) + blockCoords[2] / constants::CHUNK_SIZE;
    
    // std::cout << (unsigned int)m_chunks[m_meshedChunkPositions[i - 1]].getSkyLight(blockNumber) << " light level\n";
    integratedServer.setBlock(Position(blockCoords), blockType);

    std::vector<Position> relitChunks;
    relitChunks.push_back(chunkPosition);
    // auto tp1 = std::chrono::high_resolution_clock::now();
    // relightChunksAroundBlock(blockCoords, &relitChunks);
    // auto tp2 = std::chrono::high_resolution_clock::now();
    // std::cout << "relight took " << std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() << "us\n";

    m_accessingArrIndicesVectorsMtx.lock();
    while (m_renderThreadWaitingForArrIndicesVectors) {
        m_accessingArrIndicesVectorsMtx.unlock();
        m_renderThreadWaitingForArrIndicesVectorsMtx.lock();
        m_accessingArrIndicesVectorsMtx.lock();
        m_renderThreadWaitingForArrIndicesVectorsMtx.unlock();
    }

    auto tp1 = std::chrono::high_resolution_clock::now();
    for (auto& relitChunk : relitChunks) {
        unloadMesh(relitChunk);
        m_meshUpdates.insert(relitChunk);
    }
    auto tp2 = std::chrono::high_resolution_clock::now();
    std::cout << "unmesh took " << std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() << "us\n";
    m_accessingArrIndicesVectorsMtx.unlock();

    if (m_relableNeeded) {
        //release the chunk loader threads so that the required chunks can be remeshed
        relableCompleted = true;
        // lock release 
        std::lock_guard<std::mutex> lock(m_relableNeededMtx);
        // notify consumer when done
        m_relableNeededCV.notify_all();
    }
}

unsigned short NewClientWorld::getBlock(int* blockCoords) {
    return integratedServer.getBlock(Position(blockCoords));
}

char NewClientWorld::getNumChunkLoaderThreads() {
    return m_numChunkLoadingThreads;
}

void NewClientWorld::processMouseInput() {
    auto end = std::chrono::steady_clock::now();
    double currentTime = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - m_startTime).count() / 1000;
    if (*m_lastMousePoll == 0.0f) {
        *m_lastMousePoll = currentTime;
        return;
    }
    double DT = (currentTime - (*m_lastMousePoll)) * 0.001;
    if (DT < 0.001) {
        return;
    }
    *m_lastMousePoll = currentTime;

    int localCursorPosition[2];
    SDL_PumpEvents();
    Uint32 mouseState = SDL_GetMouseState(&localCursorPosition[0], &localCursorPosition[1]);

    //mouse input
    if (*m_playing) {
        if (*m_lastPlaying) {
            *m_yaw += (localCursorPosition[0] - m_lastMousePos[0]) * 0.05f;
            *m_pitch -= (localCursorPosition[1] - m_lastMousePos[1]) * 0.05f;
            if (*m_pitch <= -90.0f) {
                *m_pitch = -89.999f;
            }
            else if (*m_pitch >= 90.0f) {
                *m_pitch = 89.999f;
            }

            m_viewCamera->updateRotationVectors(*m_yaw, *m_pitch);
        }
        if ((abs(localCursorPosition[0] - m_windowDimensions[0] / 2) > m_windowDimensions[0] / 16)
            || (abs(localCursorPosition[1] - m_windowDimensions[1] / 2) > m_windowDimensions[1] / 16)) {
            SDL_WarpMouseInWindow(m_window, m_windowDimensions[0] / 2, m_windowDimensions[1] / 2);
            m_lastMousePos[0] = m_windowDimensions[0] / 2;
            m_lastMousePos[1] = m_windowDimensions[1] / 2;
        }
        else {
            m_lastMousePos[0] = localCursorPosition[0];
            m_lastMousePos[1] = localCursorPosition[1];
        }
    }
}

void NewClientWorld::setMouseData(double* lastMousePoll,
    bool* playing,
    bool* lastPlaying,
    float* yaw,
    float* pitch,
    int* lastMousePos,
    Camera* viewCamera,
    SDL_Window* window,
    int* windowDimensions) {
    m_lastMousePoll = lastMousePoll;
    m_playing = playing;
    m_lastPlaying = lastPlaying;
    m_yaw = yaw;
    m_pitch = pitch;
    m_lastMousePos = lastMousePos;
    m_viewCamera = viewCamera;
    m_window = window;
    m_windowDimensions = windowDimensions;
    m_startTime = std::chrono::steady_clock::now();
}

void NewClientWorld::initPlayerPos(float playerX, float playerY, float playerZ) {
    m_playerChunkPosition[0] = m_newPlayerChunkPosition[0] = m_updatingPlayerChunkPosition[0] = -1 * (playerX < 0) + playerX / constants::CHUNK_SIZE;
    m_playerChunkPosition[1] = m_newPlayerChunkPosition[1] = m_updatingPlayerChunkPosition[1] = -1 * (playerY < 0) + playerY / constants::CHUNK_SIZE;
    m_playerChunkPosition[2] = m_newPlayerChunkPosition[2] = m_updatingPlayerChunkPosition[2] = -1 * (playerZ < 0) + playerZ / constants::CHUNK_SIZE;
    m_relableNeeded = true;
}

void NewClientWorld::relightChunksAroundBlock(const int* blockCoords, std::vector<unsigned int>* relitChunks) {
    // //find the lowest block in the column that is loaded
    // int lowestChunkInWorld = m_playerChunkPosition[1] - m_renderDistance;
    // int chunkCoords[3] = { 0, 0, 0 };
    // for (char i = 0; i < 3; i++) {
    //     chunkCoords[i] = -1 * (blockCoords[i] < 0) + blockCoords[i] / constants::CHUNK_SIZE;
    // }
    // unsigned int chunkNum = getChunkNumber(chunkCoords, m_playerChunkPosition);
    // while ((chunkCoords[1] > lowestChunkInWorld) && (m_loadedChunks[chunkNum])) {
    //     chunkCoords[1] -= 1;
    //     chunkNum = getChunkNumber(chunkCoords, m_playerChunkPosition);
    // }
    // int lowestLoadedBlockInColumn = (chunkCoords[1] + 1) * constants::CHUNK_SIZE;
    // //find the lowest block in the column that has a full sky access
    // int blockPos[3];
    // for (char i = 0; i < 3; i++) {
    //     blockPos[i] = blockCoords[i];
    // }
    // blockPos[1]--;
    // while (blockPos[1] - constants::skyLightMaxValue + 1 >= lowestLoadedBlockInColumn) {
    //     unsigned char blockType = getBlock(blockPos);
    //     if (constants::dimsLight[blockType] || (!constants::transparent[blockType])) {
    //         blockPos[1]--;
    //         break;
    //     }
    //     blockPos[1]--;
    // }
    // int lowestFullySkylitBlockInColumn = blockPos[1] + 2;

    // //find the furthest blocks that the skylight could spread to and add them to a vector
    // std::vector<int> blockCoordsToBeRelit;
    // int highestAffectedBlock = blockCoords[1] + constants::skyLightMaxValue - 1;
    // int chunkLayerHeight = (-1 * (highestAffectedBlock < 0) + highestAffectedBlock / constants::CHUNK_SIZE) * constants::CHUNK_SIZE;
    // while (chunkLayerHeight >= lowestFullySkylitBlockInColumn - constants::skyLightMaxValue + 1 - constants::CHUNK_SIZE) {
    //     blockPos[0] = blockCoords[0];
    //     blockPos[1] = chunkLayerHeight;
    //     blockPos[2] = blockCoords[2];
    //     for (char i = 0; i < 3; i++) {
    //         blockCoordsToBeRelit.push_back(blockPos[i]);
    //     }
    //     blockPos[0] += constants::skyLightMaxValue - 1;
    //     for (char i = 0; i < constants::skyLightMaxValue - 1; i++) {
    //         blockPos[0]--;
    //         blockPos[2]++;
    //         for (char i = 0; i < 3; i++) {
    //             blockCoordsToBeRelit.push_back(blockPos[i]);
    //         }
    //     }
    //     for (char i = 0; i < constants::skyLightMaxValue - 1; i++) {
    //         blockPos[0]--;
    //         blockPos[2]--;
    //         for (char i = 0; i < 3; i++) {
    //             blockCoordsToBeRelit.push_back(blockPos[i]);
    //         }
    //     }
    //     for (char i = 0; i < constants::skyLightMaxValue - 1; i++) {
    //         blockPos[0]++;
    //         blockPos[2]--;
    //         for (char i = 0; i < 3; i++) {
    //             blockCoordsToBeRelit.push_back(blockPos[i]);
    //         }
    //     }
    //     for (char i = 0; i < constants::skyLightMaxValue - 1; i++) {
    //         blockPos[0]++;
    //         blockPos[2]++;
    //         for (char i = 0; i < 3; i++) {
    //             blockCoordsToBeRelit.push_back(blockPos[i]);
    //         }
    //     }
    //     chunkLayerHeight -= constants::CHUNK_SIZE;
    // }

    // //for every block that the skylight could spread to, add that chunk to a vector only if it is not there already
    // std::vector<unsigned int> chunksToBeRelit;
    // for (int i = 0; i < blockCoordsToBeRelit.size(); i += 3) {
    //     chunkCoords[0] = -1 * (blockCoordsToBeRelit[i] < 0) + blockCoordsToBeRelit[i] / constants::CHUNK_SIZE;
    //     chunkCoords[1] = -1 * (blockCoordsToBeRelit[i + 1] < 0) + blockCoordsToBeRelit[i + 1] / constants::CHUNK_SIZE;
    //     chunkCoords[2] = -1 * (blockCoordsToBeRelit[i + 2] < 0) + blockCoordsToBeRelit[i + 2] / constants::CHUNK_SIZE;
    //     chunkNum = getChunkNumber(chunkCoords, m_playerChunkPosition);
    //     if (std::find(chunksToBeRelit.begin(), chunksToBeRelit.end(), chunkNum) == chunksToBeRelit.end()) {
    //         chunksToBeRelit.push_back(chunkNum);
    //         m_chunks[m_chunkArrayIndices[chunkNum]].clearSkyLight();
    //     }
    // }

    // int numChunksRelit = 0;
    // int numSpreads = 0;
    // while (chunksToBeRelit.size() > 0) {
    //     bool neighbouringChunksToRelight[6];
    //     unsigned int neighbouringChunkIndices[6];
    //     unsigned int neighbouringChunkNumbers[6];
    //     bool neighbouringChunksLoaded = true;
    //     //check that the chunk has its neighbours loaded so that it can be lit
    //     for (char ii = 0; ii < 6; ii++) {
    //         if ((((int)(chunksToBeRelit[0]) + m_neighbouringChunkNumberOffets[ii]) < 0) 
    //             || (((int)(chunksToBeRelit[0]) + m_neighbouringChunkNumberOffets[ii]) > m_renderDiameter * m_renderDiameter * m_renderDiameter)) {
    //             neighbouringChunksLoaded = false;
    //             break;
    //         }
    //         neighbouringChunkNumbers[ii] = chunksToBeRelit[0] + m_neighbouringChunkNumberOffets[ii];
    //         if (!m_loadedChunks[neighbouringChunkNumbers[ii]]) {
    //             neighbouringChunksLoaded = false;
    //             break;
    //         }
    //         neighbouringChunkIndices[ii] = m_chunkArrayIndices[neighbouringChunkNumbers[ii]];
    //     }
    //     //if the chunk's neighbours aren't loaded, remove the chunk as it cannot be lit correctly
    //     if (!neighbouringChunksLoaded) {
    //         m_chunks[m_chunkArrayIndices[chunksToBeRelit[0]]].setSkyLightToBeOutdated();
    //         std::vector<unsigned int>::iterator it = chunksToBeRelit.begin();
    //         chunksToBeRelit.erase(it);
    //         continue;
    //     }
    //     //clear the skylight data for the chunk and relight it
    //     //m_chunks[m_chunkArrayIndices[chunksToBeRelit[0]]].clearSkyLight();
    //     m_chunks[m_chunkArrayIndices[chunksToBeRelit[0]]].calculateSkyLight(neighbouringChunkIndices, neighbouringChunksToRelight);
    //     if (std::find(relitChunks->begin(), relitChunks->end(), chunksToBeRelit[0]) == relitChunks->end()) {
    //         relitChunks->push_back(chunksToBeRelit[0]);
    //     }
    //     std::vector<unsigned int>::iterator it = chunksToBeRelit.begin();
    //     chunksToBeRelit.erase(it);
    //     //add the neighbouring chunks that were marked as needing recalculating to the queue
    //     for (char i = 0; i < 6; i++) {
    //         if (!neighbouringChunksToRelight[i]) {
    //             continue;
    //         }
    //         if (std::find(chunksToBeRelit.begin(), chunksToBeRelit.end(), neighbouringChunkNumbers[i]) == chunksToBeRelit.end()) {
    //             chunksToBeRelit.push_back(neighbouringChunkNumbers[i]);
    //             numSpreads++;
    //         }
    //     }
    //     numChunksRelit++;
    // }
    // std::cout << numChunksRelit << " chunks relit with " << numSpreads << " spreads\n";
}

}  // namespace client