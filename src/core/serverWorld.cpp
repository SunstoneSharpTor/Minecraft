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

#include "core/serverWorld.h"

#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "core/chunk.h"
#include "core/random.h"
#include "core/terrainGen.h"

ServerWorld::ServerWorld(bool singleplayer, unsigned long long seed) : m_singleplayer(singleplayer), m_seed(seed), m_nextPlayerID(0) {
    PCG_SeedRandom32(m_seed);
    seedNoise();
    // TODO:
    // set this reserved number for m_chunks to be dependant on render distance for singleplayer
    m_chunks.reserve(16777214);
    m_players.reserve(32);

    m_numChunkLoadingThreads = std::max(1u, std::min(8u, std::thread::hardware_concurrency() - 1));
}

void ServerWorld::updatePlayerPos(int playerID, int* blockPosition, float* subBlockPosition, bool waited) {
    int currentPosition[3];
    m_players.at(playerID).getChunkPosition(currentPosition);
    // if ((currentPosition[0] != std::floor((float)(blockPosition[0] / constants::CHUNK_SIZE)))
    //     || (currentPosition[1] != std::floor((float)(blockPosition[1] / constants::CHUNK_SIZE)))
    //     || (currentPosition[2] != std::floor((float)(blockPosition[2] / constants::CHUNK_SIZE)))) {
    if (waited) {
        m_playersMtx.lock();
        m_chunksMtx.lock();
        m_chunksToBeLoadedMtx.lock();
        m_chunksBeingLoadedMtx.lock();
        ServerPlayer& player = m_players.at(playerID);
        player.updatePlayerPos(blockPosition, subBlockPosition);
        // If the player has moved chunk, remove all the chunks that are out of
        // render distance from the set of loaded chunks
        Position chunkPosition;
        bool chunkOutOfRange;
        while (player.decrementNextChunk(&chunkPosition, &chunkOutOfRange)) {
            if (chunkOutOfRange && m_chunks.contains(chunkPosition)) {
                m_chunks.at(chunkPosition).decrementPlayerCount();
                if (m_chunks.at(chunkPosition).hasNoPlayers()) {
                    m_chunks.at(chunkPosition).unload();
                    m_chunks.erase(chunkPosition);
                }
            }
        }
        while (!m_chunksToBeLoaded.empty()) {
            m_chunksToBeLoaded.pop();
        }
        m_chunksBeingLoaded.clear();
        m_chunksBeingLoadedMtx.unlock();
        m_chunksToBeLoadedMtx.unlock();
        m_chunksMtx.unlock();
        m_playersMtx.unlock();
    }
}

void ServerWorld::findChunksToLoad() {
    if (m_chunksToBeLoaded.size() < m_numChunkLoadingThreads) {
        m_playersMtx.lock();
        m_chunksToBeLoadedMtx.lock();
        m_chunksBeingLoadedMtx.lock();
        m_chunksMtx.lock();
        for (auto& [playaerID, player] : m_players) {
            if (!player.allChunksLoaded()) {
                int chunkPosition[3];
                player.getNextChunkCoords(chunkPosition);
                if (m_chunks.contains(Position(chunkPosition))) {
                    m_chunks[chunkPosition].incrementPlayerCount();
                }
                else if (!m_chunksBeingLoaded.contains(Position(chunkPosition))) {
                    m_chunksToBeLoaded.emplace(chunkPosition);
                    m_chunksBeingLoaded.emplace(chunkPosition);
                }
            }
        }
        m_chunksMtx.unlock();
        m_chunksBeingLoadedMtx.unlock();
        m_chunksToBeLoadedMtx.unlock();
        m_playersMtx.unlock();
    }
}

bool ServerWorld::loadChunk(Position* chunkPosition) {
    m_chunksToBeLoadedMtx.lock();
    if (!m_chunksToBeLoaded.empty()) {
        *chunkPosition = m_chunksToBeLoaded.front();
        m_chunksToBeLoaded.pop();
        m_chunksToBeLoadedMtx.unlock();
        m_chunksMtx.lock();
        m_chunks[*chunkPosition] = { *chunkPosition, &m_chunks };
        Chunk& chunk = m_chunks.at(*chunkPosition);
        m_chunksMtx.unlock();
        TerrainGen().generateTerrain(chunk, m_seed);
        m_chunksBeingLoadedMtx.lock();
        m_chunksBeingLoaded.erase(*chunkPosition);
        m_chunksBeingLoadedMtx.unlock();
        for (auto& [playaerID, player] : m_players) {
            if (player.hasChunkLoaded(*chunkPosition)) {
                chunk.incrementPlayerCount();
            }
        }
        return true;
    }
    else {
        m_chunksToBeLoadedMtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
        return false;
    }
}

int ServerWorld::addPlayer(int* blockPosition, float* subBlockPosition, unsigned short renderDistance) {
    m_playersMtx.lock();
    m_players[m_nextPlayerID] = { m_nextPlayerID, blockPosition, subBlockPosition, renderDistance };
    m_nextPlayerID++;
    m_playersMtx.unlock();
    return m_nextPlayerID - 1;
}

unsigned char ServerWorld::getBlock(const Position& position) const {
    Position chunkPosition;
    Position chunkBlockCoords;
    chunkPosition.x = std::floor((float)position.x / constants::CHUNK_SIZE);
    chunkPosition.y = std::floor((float)position.y / constants::CHUNK_SIZE);
    chunkPosition.z = std::floor((float)position.z / constants::CHUNK_SIZE);
    // chunkPosition.x = -1 * (position.x < 0) + position.x / constants::CHUNK_SIZE;
    // chunkPosition.y = -1 * (position.y < 0) + position.y / constants::CHUNK_SIZE;
    // chunkPosition.z = -1 * (position.z < 0) + position.z / constants::CHUNK_SIZE;
    chunkBlockCoords.x = position.x - chunkPosition.x * constants::CHUNK_SIZE;
    chunkBlockCoords.y = position.y - chunkPosition.y * constants::CHUNK_SIZE;
    chunkBlockCoords.z = position.z - chunkPosition.z * constants::CHUNK_SIZE;
    unsigned int chunkBlockNum = chunkBlockCoords.y * constants::CHUNK_SIZE * constants::CHUNK_SIZE
        + chunkBlockCoords.z * constants::CHUNK_SIZE + chunkBlockCoords.x;

    auto chunkIterator = m_chunks.find(chunkPosition);

    if (chunkIterator == m_chunks.end()) {
        return 0;
    }

    return chunkIterator->second.getBlock(chunkBlockNum);
}

void ServerWorld::setBlock(const Position& position, unsigned char blockType) {
    Position chunkPosition;
    Position chunkBlockCoords;
    chunkPosition.x = std::floor((float)position.x / constants::CHUNK_SIZE);
    chunkPosition.y = std::floor((float)position.y / constants::CHUNK_SIZE);
    chunkPosition.z = std::floor((float)position.z / constants::CHUNK_SIZE);
    // chunkPosition.x = -1 * (position.x < 0) + position.x / constants::CHUNK_SIZE;
    // chunkPosition.y = -1 * (position.y < 0) + position.y / constants::CHUNK_SIZE;
    // chunkPosition.z = -1 * (position.z < 0) + position.z / constants::CHUNK_SIZE;
    chunkBlockCoords.x = position.x - chunkPosition.x * constants::CHUNK_SIZE;
    chunkBlockCoords.y = position.y - chunkPosition.y * constants::CHUNK_SIZE;
    chunkBlockCoords.z = position.z - chunkPosition.z * constants::CHUNK_SIZE;
    unsigned int chunkBlockNum = chunkBlockCoords.y * constants::CHUNK_SIZE * constants::CHUNK_SIZE
        + chunkBlockCoords.z * constants::CHUNK_SIZE + chunkBlockCoords.x;

    auto chunkIterator = m_chunks.find(chunkPosition);

    if (chunkIterator == m_chunks.end()) {
        return;
    }

    chunkIterator->second.setBlock(chunkBlockNum, blockType);
}


unsigned char ServerWorld::getSkyLight(const Position& position) const {
    Position chunkPosition;
    Position chunkBlockCoords;
    chunkPosition.x = std::floor((float)position.x / constants::CHUNK_SIZE);
    chunkPosition.y = std::floor((float)position.y / constants::CHUNK_SIZE);
    chunkPosition.z = std::floor((float)position.z / constants::CHUNK_SIZE);
    // chunkPosition.x = -1 * (position.x < 0) + position.x / constants::CHUNK_SIZE;
    // chunkPosition.y = -1 * (position.y < 0) + position.y / constants::CHUNK_SIZE;
    // chunkPosition.z = -1 * (position.z < 0) + position.z / constants::CHUNK_SIZE;
    chunkBlockCoords.x = position.x - chunkPosition.x * constants::CHUNK_SIZE;
    chunkBlockCoords.y = position.y - chunkPosition.y * constants::CHUNK_SIZE;
    chunkBlockCoords.z = position.z - chunkPosition.z * constants::CHUNK_SIZE;
    unsigned int chunkBlockNum = chunkBlockCoords.y * constants::CHUNK_SIZE * constants::CHUNK_SIZE
        + chunkBlockCoords.z * constants::CHUNK_SIZE + chunkBlockCoords.x;

    auto chunkIterator = m_chunks.find(chunkPosition);

    if (chunkIterator == m_chunks.end()) {
        return 0;
    }

    return chunkIterator->second.getSkyLight(chunkBlockNum);
}