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

#pragma once

#include "client/renderer.h"
#include "client/vertexBuffer.h"
#include "client/indexBuffer.h"
#include "client/vertexArray.h"
#include "client/shader.h"
#include "client/texture.h"
#include "client/camera.h"
#include "core/chunk.h"
#include "core/position.h"
#include "core/serverWorld.h"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "enet/enet.h"

namespace client {

class NewClientWorld {
private:
	bool m_singleplayer;
	unsigned short m_renderDistance;
	unsigned short m_renderDiameter;
	int m_playerChunkPosition[3];
	int m_newPlayerChunkPosition[3];
	int m_updatingPlayerChunkPosition[3];
	std::unordered_set<Position> m_meshUpdates; //stores chunks that have to have their meshes rebuilt after a block update
	//stores the location of each chunk in the array m_chunks, ordered by chunk number
	Position m_neighbouringChunkOffets[6];
	Position m_neighbouringChunkIncludingDiaganalOffsets[27];
	unsigned short m_numChunkLoadingThreads;
	bool m_renderingFrame;
	float m_meshedChunksDistance;
	float m_fogDistance;
	double m_timeByDTs;
	unsigned long long m_seed;

	//mouse polling info
	std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::_V2::steady_clock::duration> m_startTime;
	double* m_lastMousePoll;
	bool* m_playing;
	bool* m_lastPlaying;
	float* m_yaw;
	float* m_pitch;
	int* m_lastMousePos;
	Camera* m_viewCamera;
	SDL_Window* m_window;
	int* m_windowDimensions;

	std::unordered_map<Position, VertexArray*> m_chunkVertexArrays;
	std::unordered_map<Position, VertexBuffer*> m_chunkVertexBuffers;
	std::unordered_map<Position, IndexBuffer*> m_chunkIndexBuffers;
	std::unordered_map<Position, VertexArray*> m_chunkWaterVertexArrays;
	std::unordered_map<Position, VertexBuffer*> m_chunkWaterVertexBuffers;
	std::unordered_map<Position, IndexBuffer*> m_chunkWaterIndexBuffers;
	VertexBuffer* m_emptyVertexBuffer;
	IndexBuffer* m_emptyIndexBuffer;
	VertexArray* m_emptyVertexArray;
	std::unordered_set<Position> m_meshedChunks;
	std::unordered_set<Position> m_unmeshedChunks;

	//mesh building data - this is stored at class-level because it allows it to be
	//accessed from multiple threads
	unsigned int* m_numChunkVertices; //array to allow for each mesh-building thread to have its own value
	unsigned int* m_numChunkWaterVertices; //array to allow for each mesh-building thread to have its own value
	unsigned int* m_numChunkIndices; //array to allow for each mesh-building thread to have its own value
	unsigned int* m_numChunkWaterIndices; //array to allow for each mesh-building thread to have its own value
	Position* m_chunkPosition; //array to allow for each mesh-building thread to have its own value
	float** m_chunkVertices; //2d array to allow for each mesh-building thread to have its own array
	unsigned int** m_chunkIndices; //2d array to allow for each mesh-building thread to have its own array
	float** m_chunkWaterVertices; //2d array to allow for each mesh-building thread to have its own array
	unsigned int** m_chunkWaterIndices; //2d array to allow for each mesh-building thread to have its own array

	//communication
	std::mutex* m_chunkMeshReadyMtx;
	std::condition_variable* m_chunkMeshReadyCV;
	std::mutex m_relableNeededMtx;
	std::condition_variable m_relableNeededCV;
	std::mutex m_accessingArrIndicesVectorsMtx;
	std::mutex m_renderThreadWaitingForArrIndicesVectorsMtx;
	std::mutex m_unmeshedChunksMtx;
	bool m_renderThreadWaitingForArrIndicesVectors;
	bool* m_chunkMeshReady;
	bool m_relableNeeded;
	bool* m_relableOccurred;
	bool* m_threadWaiting;
	int m_mouseCalls;
	int m_numRelights;

	//chunk number is a number assigned to each chunk within render distance (between
	//0 and m_numChunks) that allows the chunks to be represented in a 1d array
	unsigned int getChunkNumber(int* chunkCoords, int* playerCoords);

	void getChunkCoords(int* chunkCoords, unsigned int chunkNumber, int* playerCoords);
	void unloadMesh(const Position& chunkPosition);
	bool chunkHasNeighbours(const Position& chunkPosition);
	void addChunkMesh(const Position& chunkPosition, char threadNum);
	void uploadChunkMesh(char threadNum);
	void unloadAndRelableChunks();
	void relightChunksAroundBlock(const int* blockCoords, std::vector<unsigned int>* relitChunks);

public:
	ServerWorld integratedServer;

	NewClientWorld(unsigned short renderDistance, unsigned long long seed, bool singleplayer, ENetPeer* peer, ENetHost* client);
	void renderChunks(Renderer mainRenderer, Shader& blockShader, Shader& waterShader, glm::mat4 viewMatrix, glm::mat4 projMatrix, int* playerBlockPosition, float aspectRatio, float fov, double DT);
	void loadChunksAroundPlayer(char threadNum);
	void buildMeshesForNewChunksWithNeighbours(char threadNum);
	unsigned char shootRay(glm::vec3 startSubBlockPos, int* startBlockPosition, glm::vec3 direction, int* breakBlockCoords, int* placeBlockCoords);
	void replaceBlock(int* blockCoords, unsigned char blockType);
	unsigned short getBlock(int* blockCoords);
	inline int getRenderDistance() {
		return m_renderDistance;
	}
	void doRenderThreadJobs();
	void relableChunksIfNeeded();
	void updatePlayerPos(float playerX, float playerY, float playerZ);
	void initPlayerPos(float playerX, float playerY, float playerZ);
	char getNumChunkLoaderThreads();
	void setMouseData(double* lastMousePoll,
					  bool* playing,
					  bool* lastPlaying,
					  float* yaw,
					  float* pitch,
					  int* lastMousePos,
					  Camera* viewCamera,
					  SDL_Window* window,
					  int* windowDimensions);
	void processMouseInput();
};

}  // namespace client