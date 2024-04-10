#include "chunk.h"
#include "random.h"

#include <iostream>
#include <cmath>
#include <queue>
#include <thread>

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

using namespace std;

mutex chunk::s_checkingNeighbouringRelights;

//define the face positions constant:
const float chunk::m_cubeTextureCoordinates[48] = { 0, 0,
													1, 0,
													1, 1,
													0, 1,
													
													0, 0,
													1, 0,
													1, 1,
													0, 1,
													
													0, 0,
													1, 0,
													1, 1,
													0, 1,
													
													0, 0,
													1, 0,
													1, 1,
													0, 1,
													
													0, 0,
													1, 0,
													1, 1,
													0, 1,
													
													0, 0,
													1, 0,
													1, 1,
													0, 1 };

const float chunk::m_xTextureCoordinates[32] = { 0, 0,
												 1, 0,
												 1, 1,
												 0, 1,
												 
												 0, 0,
												 1, 0,
												 1, 1,
												 0, 1,
												 
												 0, 0,
												 1, 0,
												 1, 1,
												 0, 1,
												 
												 0, 0,
												 1, 0,
												 1, 1,
												 0, 1 };

const short chunk::m_blockIdToTextureNum[48] = { 0, 0, 0, 0, 0, 0, //air
												 0, 0, 0, 0, 0, 0, //dirt
												 2, 2, 2, 2, 0, 1, //grass
												 3, 3, 3, 3, 3, 3, //stone
												 4, 4, 4, 4, 4, 4, //water
												 36, 36, 36, 36, 37, 37, //oak log
												 38, 38, 38, 38, 38, 38, //oak leaves
												 39, 39, 39, 39, 39, 39 //tall grass
												 };

const short chunk::m_neighbouringBlocks[6] = { -(constants::CHUNK_SIZE * constants::CHUNK_SIZE), -constants::CHUNK_SIZE, -1, 1, constants::CHUNK_SIZE, (constants::CHUNK_SIZE * constants::CHUNK_SIZE) };

const short chunk::m_neighbouringBlocksX[6] = { 0, 0, -1, 1, 0, 0 };

const short chunk::m_neighbouringBlocksY[6] = { -1, 0, 0, 0, 0, 1 };

const short chunk::m_neighbouringBlocksZ[6] = { 0, -1, 0, 0, 1, 0 };

const int chunk::m_neighbouringChunkBlockOffsets[6] = { constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1), constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1), constants::CHUNK_SIZE - 1, -(constants::CHUNK_SIZE - 1), -(constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) };

const short chunk::m_adjacentBlocksToFaceOffests[48] = { -1 - constants::CHUNK_SIZE, -constants::CHUNK_SIZE, -constants::CHUNK_SIZE + 1, 1, 1 + constants::CHUNK_SIZE, constants::CHUNK_SIZE, constants::CHUNK_SIZE - 1, -1,
														  1 - (constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE) - 1, -1, -1 + constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE + 1, 1,
														  -constants::CHUNK_SIZE - (constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE) + constants::CHUNK_SIZE, constants::CHUNK_SIZE, constants::CHUNK_SIZE + constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE - constants::CHUNK_SIZE, -constants::CHUNK_SIZE,
														  constants::CHUNK_SIZE - (constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE) - constants::CHUNK_SIZE, -constants::CHUNK_SIZE, -constants::CHUNK_SIZE + constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE + constants::CHUNK_SIZE, constants::CHUNK_SIZE,
														  -1 - (constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE), -(constants::CHUNK_SIZE * constants::CHUNK_SIZE) + 1, 1, 1 + constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE, constants::CHUNK_SIZE * constants::CHUNK_SIZE - 1, -1,
														  -1 + constants::CHUNK_SIZE, constants::CHUNK_SIZE, constants::CHUNK_SIZE + 1, 1, 1 - constants::CHUNK_SIZE, -constants::CHUNK_SIZE, -constants::CHUNK_SIZE - 1, -1 };

const short chunk::m_adjacentBlocksToFaceOffestsX[48] = { -1, 0, 1, 1, 1, 0, -1, -1,
														  1, 0, -1, -1, -1, 0, 1, 1,
														  0, 0, 0, 0, 0, 0, 0, 0,
														  0, 0, 0, 0, 0, 0, 0, 0,
														  -1, 0, 1, 1, 1, 0, -1, -1,
														  -1, 0, 1, 1, 1, 0, -1, -1 };

const short chunk::m_adjacentBlocksToFaceOffestsY[48] = { 0, 0, 0, 0, 0, 0, 0, 0,
														  -1, -1, -1, 0, 1, 1, 1, 0,
														  -1, -1, -1, 0, 1, 1, 1, 0,
														  -1, -1, -1, 0, 1, 1, 1, 0,
														  -1, -1, -1, 0, 1, 1, 1, 0,
														  0, 0, 0, 0, 0, 0, 0, 0 };

const short chunk::m_adjacentBlocksToFaceOffestsZ[48] = { -1, -1, -1, 0, 1, 1, 1, 0,
														  0, 0, 0, 0, 0, 0, 0, 0,
														  -1, 0, 1, 1, 1, 0, -1, -1,
														  1, 0, -1, -1, -1, 0, 1, 1,
														  0, 0, 0, 0, 0, 0, 0, 0,
														  1, 1, 1, 0, -1, -1, -1, 0 };

//define the terrain heights constants
const float chunk::m_continentalnessNoiseVals[10] = { -1.0f, -0.2f, 1.0f }; //{ -1.0f, -0.4f, -0.35f, -0.2f, -0.18f, -0.17f, -0.14f, 0.2f, 1.0f };
const float chunk::m_continentalnessTerrainHeights[10] = { -200.0f, -10.0f, 24.0f }; //{ 0.0f, 0.0f, 40.0f, 40.0f, 90.0f, 90.0f, 92.0f, 96.0f, 100.0f };
const float chunk::m_erosionNoiseVals[10] = { -1.0f, 0.0f, 1.0f };
const float chunk::m_erosionTerrainHeights[10] = { 5.0f, -5.0f, -20.0f };
const float chunk::m_smallErosionNoiseVals[10] = { -1.0f, 1.0f };
const float chunk::m_smallErosionTerrainHeights[10] = { 10.0f, -50.0f };
const float chunk::m_peaksAndValleysNoiseVals[10] = { -1.0f, 1.0f };
const float chunk::m_peaksAndValleysTerrainHeights[10] = { -9.0f, 12.6f };
const float chunk::m_bumpsNoiseVals[10] = { -1.0f, 1.0f };
const float chunk::m_bumpsTerrainHeights[10] = { -30.0f, 100.0f };

const float chunk::m_beachTerrainHeights[10] = { -1000, -75.0f, -40.0f, -10.0f , 1000.0f };
const float chunk::m_beachNoiseVals[10] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

chunk::chunk(int x, int y, int z, worldInfo wio) {
	inUse = true;
	m_singleBlockType = false;
	m_singleSkyLightVal = false;
	m_skyLightUpToDate = false;
	m_calculatingSkylight = false;
	m_worldInfo = wio;

	m_blocks = new unsigned char[constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
	m_skyLight = new unsigned char[(constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE + 1) / 2];

	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
	generateTerrain();
}

chunk::chunk(worldInfo wio) {
	inUse = false;
	m_singleBlockType = false;
	m_singleSkyLightVal = false;
	m_skyLightUpToDate = false;
	m_calculatingSkylight = false;
	m_worldInfo = wio;

	m_blocks = new unsigned char[0];
	m_skyLight = new unsigned char[0];

	m_position[0] = 0;
	m_position[1] = 0;
	m_position[2] = 0;
}

chunk::chunk() {
	inUse = false;
	m_singleBlockType = false;
	m_singleSkyLightVal = false;
	m_skyLightUpToDate = false;
	m_calculatingSkylight = false;
	m_worldInfo = worldInfo();

	m_blocks = new unsigned char[0];
	m_skyLight = new unsigned char[0];

	m_position[0] = 0;
	m_position[1] = 0;
	m_position[2] = 0;
}

void chunk::recreate(int x, int y, int z) {
	inUse = true;
	m_singleBlockType = false;
	m_singleSkyLightVal = false;
	m_calculatingSkylight = false;
	delete[] m_blocks;
	delete[] m_skyLight;
	m_blocks = new unsigned char[constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
	m_skyLight = new unsigned char[(constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE + 1) / 2];

	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
	generateTerrain();
}

void chunk::setWorldInfo(worldInfo wio) {
	m_worldInfo = wio;
}

void chunk::generateTerrain() {
	m_skyLightUpToDate = false;
	for (unsigned int blockNum = 0; blockNum < (constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE + 1) / 2; blockNum++) {
		m_skyLight[blockNum] = 0;
	}

	//calculate coordinates of the chunk
	int chunkMinCoords[3];
	int chunkMaxCoords[3];
	for (unsigned char i = 0; i < 3; i++) {
		chunkMinCoords[i] = m_position[i] * constants::CHUNK_SIZE;
		chunkMaxCoords[i] = chunkMinCoords[i] + constants::CHUNK_SIZE;
	}

	m_singleBlockType = true;
	int heightMap[(constants::CHUNK_SIZE + 4) * (constants::CHUNK_SIZE + 4)];
	int blockPos[3];
	unsigned int lastBlockTypeInChunk = 0;
	for (int z = -2; z < constants::CHUNK_SIZE + 2; z++) {
		for (int x = -2; x < constants::CHUNK_SIZE + 2; x++) {
			//generate height map for terrain using simplex noise
			float continentalness = glm::simplex(glm::vec2(m_position[0] * constants::CHUNK_SIZE + x + constants::WORLD_BORDER_DISTANCE, m_position[2] * constants::CHUNK_SIZE + z + constants::WORLD_BORDER_DISTANCE) * (1.0f / 3000.0f));
			float continentalnessHeight = getTerrainHeight(continentalness, m_continentalnessNoiseVals, m_continentalnessTerrainHeights);
			float erosion = glm::simplex(glm::vec2(m_position[0] * constants::CHUNK_SIZE + x + constants::WORLD_BORDER_DISTANCE, m_position[2] * constants::CHUNK_SIZE + z + constants::WORLD_BORDER_DISTANCE) * (1.0f / 1600.0f));
			float erosionHeight = getTerrainHeight(erosion * (continentalnessHeight + 88.0f) / 112.0f, m_erosionNoiseVals, m_erosionTerrainHeights);
			float smallErosion = glm::simplex(glm::vec2(m_position[0] * constants::CHUNK_SIZE + x + constants::WORLD_BORDER_DISTANCE, m_position[2] * constants::CHUNK_SIZE + z + constants::WORLD_BORDER_DISTANCE) * (1.0f / 800.0f));
			float smallErosionHeight = getTerrainHeight(smallErosion, m_smallErosionNoiseVals, m_smallErosionTerrainHeights);
			float totalErosionHeight = erosionHeight + smallErosionHeight;
			float peaksAndValleys = glm::simplex(glm::vec2(m_position[0] * constants::CHUNK_SIZE + x + constants::WORLD_BORDER_DISTANCE, m_position[2] * constants::CHUNK_SIZE + z + constants::WORLD_BORDER_DISTANCE) * (1.0f / 1200.0f));
			float peaksAndValleysHeight = getTerrainHeight(peaksAndValleys * ((totalErosionHeight + 120.0f) / 165.0f), m_peaksAndValleysNoiseVals, m_peaksAndValleysTerrainHeights) * (continentalnessHeight + 88.0f) / 112.0f;
			peaksAndValleysHeight *= peaksAndValleysHeight * peaksAndValleysHeight * -(smallErosion - 1.0f) / 2.0f;
			
			float bumps = glm::simplex(glm::vec2(m_position[0] * constants::CHUNK_SIZE + x + constants::WORLD_BORDER_DISTANCE, m_position[2] * constants::CHUNK_SIZE + z + constants::WORLD_BORDER_DISTANCE) * (1.0f / 80.0f));

			float workingHeight = continentalnessHeight + totalErosionHeight + peaksAndValleysHeight;

			float bumpsHeight = getTerrainHeight(bumps * ((erosion + 0.65f) * (getTerrainHeight(workingHeight, m_beachTerrainHeights, m_beachNoiseVals) * 5.0f) + (peaksAndValleysHeight / 15.0f)) / 40.0f, m_bumpsNoiseVals, m_bumpsTerrainHeights);

			workingHeight += bumpsHeight;

			int height = workingHeight;
			heightMap[(z + 2) * (constants::CHUNK_SIZE + 4) + (x + 2)] = height;

			if ((x >= 0) && (x < constants::CHUNK_SIZE) && (z >= 0) && (z < constants::CHUNK_SIZE)) {
				unsigned int blockNum = z * constants::CHUNK_SIZE + x;
				for (int y = chunkMinCoords[1]; y < chunkMaxCoords[1]; y++) {
					if (y > height) {
						if (y > 0) {
							m_blocks[blockNum] = 0;
							m_skyLight[blockNum / 2] |= (15 << (4 * (blockNum % 2)));
						}
						else {
							m_blocks[blockNum] = 4;
							m_skyLight[blockNum / 2] |= ((15 + max(y - 1, -15)) << (4 * (blockNum % 2)));
						}
					}
					else if (y == height) {
						if (y < 0) {
							m_blocks[blockNum] = 1;
						}
						else {
							m_blocks[blockNum] = 2;
						}
					}
					else if (y > (height - 3)) {
						m_blocks[blockNum] = 1;
					}
					else {
						m_blocks[blockNum] = 3;
					}
					m_singleBlockType &= ((blockNum == 0) || (m_blocks[blockNum] == lastBlockTypeInChunk));
					lastBlockTypeInChunk = m_blocks[blockNum];
					blockNum += constants::CHUNK_SIZE * constants::CHUNK_SIZE;
				}
			}
		}
	}

	//add trees
	unsigned int blockNum = 0;
	for (int z = chunkMinCoords[2] - 2; z < chunkMaxCoords[2] + 2; z++) {
		for (int x = chunkMinCoords[0] - 2; x < chunkMaxCoords[0] + 2; x++) {
			if ((heightMap[blockNum] >= 0) && (chunkMinCoords[1] < (heightMap[blockNum] + 8)) && ((chunkMaxCoords[1]) > heightMap[blockNum])) {
				//convert the 2d block coordinate into a unique integer that can be used as the seed for the PRNG
				int blockNumberInWorld;
				if (z > x) {
					blockNumberInWorld = ((z + constants::WORLD_BORDER_DISTANCE) + 2) * (z + constants::WORLD_BORDER_DISTANCE) - (x + constants::WORLD_BORDER_DISTANCE);
				}
				else {
					blockNumberInWorld = (x + constants::WORLD_BORDER_DISTANCE) * (x + constants::WORLD_BORDER_DISTANCE) + (z + constants::WORLD_BORDER_DISTANCE);
				}
				int random = PCG_Hash(blockNumberInWorld) % 40u;
				if (random == 0) {
					bool nearbyTree = false;
					for (int checkZ = z - 2; checkZ <= z; checkZ++) {
						for (int checkX = x - 2; checkX <= x + 2; checkX++) {
							if (checkZ > checkX) {
								blockNumberInWorld = ((checkZ + constants::WORLD_BORDER_DISTANCE) + 2) * (checkZ + constants::WORLD_BORDER_DISTANCE) - (checkX + constants::WORLD_BORDER_DISTANCE);
							}
							else {
								blockNumberInWorld = (checkX + constants::WORLD_BORDER_DISTANCE) * (checkX + constants::WORLD_BORDER_DISTANCE) + (checkZ + constants::WORLD_BORDER_DISTANCE);
							}
							int random = PCG_Hash(blockNumberInWorld) % 40u;
							if ((random == 0) && (!((checkX == x) && (checkZ == z)))) {
								nearbyTree = true;
								checkX = x + 3;
								checkZ = z + 1;
							}
						}
					}
					if (!nearbyTree) {
						//position has been selected for adding a tree
						//set the blocks for the tree
						int bottomOfTree[3];
						bottomOfTree[1] = (heightMap[blockNum] + 1);
						bottomOfTree[0] = x;
						bottomOfTree[2] = z;
						int treeBlockPos[3];
						for (unsigned char i = 0; i < 3; i++) {
							treeBlockPos[i] = bottomOfTree[i];
						}
						treeBlockPos[1] -= 1;
						int treeBlockNum = (treeBlockPos[0] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE + ((treeBlockPos[1] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE * constants::CHUNK_SIZE + ((treeBlockPos[2] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE;
						//build the trunk
						int trunkHeight = 3 + PCG_Hash(blockNumberInWorld) % 3;
						for (int logHeight = -1; logHeight < trunkHeight + 2; logHeight++) {
							//if the block is in the chunk
							if (((treeBlockPos[1] >= chunkMinCoords[1]) && (treeBlockPos[1] < chunkMaxCoords[1])) && ((treeBlockPos[0] >= chunkMinCoords[0]) && (treeBlockPos[0] < chunkMaxCoords[0])) && ((treeBlockPos[2] >= chunkMinCoords[2]) && (treeBlockPos[2] < chunkMaxCoords[2]))) {
								m_blocks[treeBlockNum] = 1 + (logHeight >= 0) * (4 + (logHeight >= trunkHeight));
								m_singleBlockType = false;
								m_skyLight[treeBlockNum / 2] &= (15 << (4 * !(treeBlockNum % 2))) | (0b11111111 * (logHeight >= trunkHeight));
							}
							treeBlockPos[1]++;
							treeBlockNum += constants::CHUNK_SIZE * constants::CHUNK_SIZE;
							treeBlockNum = treeBlockNum % (constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE);
						}
						//build the upper leaves
						treeBlockPos[0] -= 1;
						treeBlockPos[1] -= 2;
						for (unsigned char i = 0; i < 4; i++) {
							for (unsigned char ii = 0; ii < 2; ii++) {
								treeBlockNum = (treeBlockPos[0] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE + ((treeBlockPos[1] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE * constants::CHUNK_SIZE + ((treeBlockPos[2] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE;
								if (((treeBlockPos[1] >= chunkMinCoords[1]) && (treeBlockPos[1] < chunkMaxCoords[1])) && ((treeBlockPos[0] >= chunkMinCoords[0]) && (treeBlockPos[0] < chunkMaxCoords[0])) && ((treeBlockPos[2] >= chunkMinCoords[2]) && (treeBlockPos[2] < chunkMaxCoords[2]))) {
									m_blocks[treeBlockNum] = 6;
									m_singleBlockType = false;
								}
								treeBlockPos[1]++;
							}
							treeBlockPos[1] -= 2;
							treeBlockPos[0] += 1 + (i / 2) * -2;
							treeBlockPos[2] += 1 + ((i + 1) / 2) * -2;
						}
						//build the lower leaves
						treeBlockPos[0] += 4;
						treeBlockPos[1] -= 2;
						for (treeBlockPos[2] = bottomOfTree[2] - 2; treeBlockPos[2] < bottomOfTree[2] + 3; treeBlockPos[2]++) {
							for (treeBlockPos[0] = bottomOfTree[0] - 2; treeBlockPos[0] < bottomOfTree[0] + 3; treeBlockPos[0]++) {
								for (unsigned char ii = 0; ii < 2; ii++) {
									treeBlockNum = (treeBlockPos[0] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE + ((treeBlockPos[1] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE * constants::CHUNK_SIZE + ((treeBlockPos[2] + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE;
									if (((treeBlockPos[1] >= chunkMinCoords[1]) && (treeBlockPos[1] < chunkMaxCoords[1])) && ((treeBlockPos[0] >= chunkMinCoords[0]) && (treeBlockPos[0] < chunkMaxCoords[0])) && ((treeBlockPos[2] >= chunkMinCoords[2]) && (treeBlockPos[2] < chunkMaxCoords[2]))) {
										if (m_blocks[treeBlockNum] == 0) {
											m_blocks[treeBlockNum] = 6;
											m_singleBlockType = false;
										}
									}
									treeBlockPos[1]++;
								}
								treeBlockPos[1] -= 2;
							}
						}
					}
				}
			}
			blockNum++;
		}
	}

	//add grass
	short heightMapIndex;
	short chunkZ = 0;
	short chunkX;
	for (int z = chunkMinCoords[2]; z < chunkMaxCoords[2]; z++) {
		chunkX = 0;
		for (int x = chunkMinCoords[0]; x < chunkMaxCoords[0]; x++) {
			heightMapIndex = (constants::CHUNK_SIZE + 4) * 2 + chunkZ * (constants::CHUNK_SIZE + 4) + chunkX + 2;
			if ((heightMap[heightMapIndex] >= 0) && (chunkMinCoords[1] < (heightMap[heightMapIndex] + 2)) && ((chunkMaxCoords[1]) > heightMap[heightMapIndex])) {
				//convert the 2d block coordinate into a unique integer that can be used as the seed for the PRNG
				int blockNumberInWorld;
				if (z > x) {
					blockNumberInWorld = ((z + constants::WORLD_BORDER_DISTANCE) + 2) * (z + constants::WORLD_BORDER_DISTANCE) - (x + constants::WORLD_BORDER_DISTANCE);
				}
				else {
					blockNumberInWorld = (x + constants::WORLD_BORDER_DISTANCE) * (x + constants::WORLD_BORDER_DISTANCE) + (z + constants::WORLD_BORDER_DISTANCE);
				}
				int random = PCG_Hash(blockNumberInWorld) % 3u;
				if (random == 0) {
					blockNum = (x + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE + ((heightMap[heightMapIndex] + 1 + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE * constants::CHUNK_SIZE + ((z + constants::BORDER_DISTANCE_U_B) % constants::CHUNK_SIZE) * constants::CHUNK_SIZE;
					if (((heightMap[heightMapIndex] + 1 >= chunkMinCoords[1]) && (heightMap[heightMapIndex] + 1 < chunkMaxCoords[1])) && ((x >= chunkMinCoords[0]) && (x < chunkMaxCoords[0])) && ((z >= chunkMinCoords[2]) && (z < chunkMaxCoords[2]))) {
						if (m_blocks[blockNum] == 0) {
							m_blocks[blockNum] = 7;
							m_singleBlockType = false;
						}
					}
				}
			}
			chunkX++;
		}
		chunkZ++;
	}

	//if the chunk is made up of a single block, compress it
	if (m_singleBlockType) {
		unsigned short blockType = m_blocks[0];
		delete[] m_blocks;
		m_blocks = new unsigned char[1];
		m_blocks[0] = blockType;
	}
}

void chunk::findBlockCoordsInWorld(int* blockPos, unsigned int block) {
	int chunkCoords[3];
	getChunkPosition(chunkCoords);
	blockPos[0] = block % constants::CHUNK_SIZE + chunkCoords[0] * constants::CHUNK_SIZE;
	blockPos[1] = block / (constants::CHUNK_SIZE * constants::CHUNK_SIZE) + chunkCoords[1] * constants::CHUNK_SIZE;
	blockPos[2] = block / constants::CHUNK_SIZE % constants::CHUNK_SIZE + chunkCoords[2] * constants::CHUNK_SIZE;
}

void chunk::findBlockCoordsInChunk(float* blockPos, unsigned int block) {
	blockPos[0] = block % constants::CHUNK_SIZE;
	blockPos[1] = block / (constants::CHUNK_SIZE * constants::CHUNK_SIZE);
	blockPos[2] = block / constants::CHUNK_SIZE % constants::CHUNK_SIZE;
}

void chunk::findBlockCoordsInChunk(unsigned short* blockPos, unsigned int block) {
	blockPos[0] = block % constants::CHUNK_SIZE;
	blockPos[1] = block / (constants::CHUNK_SIZE * constants::CHUNK_SIZE);
	blockPos[2] = block / constants::CHUNK_SIZE % constants::CHUNK_SIZE;
}

void chunk::addFaceToMesh(float* vertices, unsigned int* numVertices, unsigned int* indices, unsigned int* numIndices, float* waterVertices, unsigned int* numWaterVertices, unsigned int* waterIndices, unsigned int* numWaterIndices, unsigned int block, short neighbouringBlock) {
	float blockPos[3];
	int neighbouringBlockPos[3];
	findBlockCoordsInChunk(blockPos, block);
	neighbouringBlockPos[0] = m_position[0] * constants::CHUNK_SIZE + blockPos[0] + m_neighbouringBlocksX[neighbouringBlock];
	neighbouringBlockPos[1] = m_position[1] * constants::CHUNK_SIZE + blockPos[1] + m_neighbouringBlocksY[neighbouringBlock];
	neighbouringBlockPos[2] = m_position[2] * constants::CHUNK_SIZE + blockPos[2] + m_neighbouringBlocksZ[neighbouringBlock];

	if (constants::cubeMesh[m_blocks[block]]) {
		short firstPositionIndex;
		short firstAdjacentBlockIndex;
		short textureNum;
		short neighbouringBlockCoordsOffset;
		switch (neighbouringBlock) {
			case 0:
				firstPositionIndex = 48;
				firstAdjacentBlockIndex = 0;
				textureNum = 4;
				neighbouringBlockCoordsOffset = 0;
				break;
			case 1:
				firstPositionIndex = 0;
				firstAdjacentBlockIndex = 8;
				textureNum = 2;
				neighbouringBlockCoordsOffset = 1;
				break;
			case 2:
				firstPositionIndex = 24;
				firstAdjacentBlockIndex = 16;
				textureNum = 0;
				neighbouringBlockCoordsOffset = 2;
				break;
			case 3:
				firstPositionIndex = 36;
				firstAdjacentBlockIndex = 24;
				textureNum = 1;
				neighbouringBlockCoordsOffset = 3;
				break;
			case 4:
				firstPositionIndex = 12;
				firstAdjacentBlockIndex = 32;
				textureNum = 3;
				neighbouringBlockCoordsOffset = 4;
				break;
			default: //5
				firstPositionIndex = 60;
				firstAdjacentBlockIndex = 40;
				textureNum = 5;
				neighbouringBlockCoordsOffset = 5;
				break;
		}

		if (m_blocks[block] == 4) {
			//TODO
			//find whether there is a block above the water to see if the water should be rendered as a full block or a surface block
			bool waterAbove;
			if (block >= (constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))) {
				int blockCoords[3];
				//wip
			}

			float texCoords[8];
			getTextureCoordinates(texCoords, m_blockIdToTextureNum[m_blocks[block] * 6 + textureNum]);
			for (short vertex = 0; vertex < 4; vertex++) {
				for (short element = 0; element < 3; element++) {
					waterVertices[*numWaterVertices] = constants::CUBE_FACE_POSITIONS[firstPositionIndex + vertex * 3 + element] + blockPos[element];
					(*numWaterVertices)++;
				}
				waterVertices[*numWaterVertices] = texCoords[vertex * 2];
				(*numWaterVertices)++;
				waterVertices[*numWaterVertices] = texCoords[vertex * 2 + 1];
				(*numWaterVertices)++;
				waterVertices[*numWaterVertices] = (1.0f / 16.0f) * (getWorldSkyLight(neighbouringBlockPos) + 1);
				(*numWaterVertices)++;
			}

			//index buffer
			int trueNumVertices = *numWaterVertices / 6;
			waterIndices[*numWaterIndices] = trueNumVertices - 4;
			(*numWaterIndices)++;
			waterIndices[*numWaterIndices] = trueNumVertices - 3;
			(*numWaterIndices)++;
			waterIndices[*numWaterIndices] = trueNumVertices - 2;
			(*numWaterIndices)++;
			waterIndices[*numWaterIndices] = trueNumVertices - 4;
			(*numWaterIndices)++;
			waterIndices[*numWaterIndices] = trueNumVertices - 2;
			(*numWaterIndices)++;
			waterIndices[*numWaterIndices] = trueNumVertices - 1;
			(*numWaterIndices)++;

			if (neighbouringBlock == 5) {
				firstPositionIndex = 48;
				firstAdjacentBlockIndex = 0;
				textureNum = 4;
				neighbouringBlockCoordsOffset = 0;
				blockPos[1]++;

				getTextureCoordinates(texCoords, m_blockIdToTextureNum[m_blocks[block] * 6 + textureNum]);
				for (short vertex = 0; vertex < 4; vertex++) {
					for (short element = 0; element < 3; element++) {
						waterVertices[*numWaterVertices] = constants::CUBE_FACE_POSITIONS[firstPositionIndex + vertex * 3 + element] + blockPos[element];
						(*numWaterVertices)++;
					}
					waterVertices[*numWaterVertices] = texCoords[vertex * 2];
					(*numWaterVertices)++;
					waterVertices[*numWaterVertices] = texCoords[vertex * 2 + 1];
					(*numWaterVertices)++;
					waterVertices[*numWaterVertices] = (1.0f / 16.0f) * (getWorldSkyLight(neighbouringBlockPos) + 1);
					(*numWaterVertices)++;
				}


				//index buffer
				trueNumVertices = *numWaterVertices / 6;
				waterIndices[*numWaterIndices] = trueNumVertices - 4;
				(*numWaterIndices)++;
				waterIndices[*numWaterIndices] = trueNumVertices - 3;
				(*numWaterIndices)++;
				waterIndices[*numWaterIndices] = trueNumVertices - 2;
				(*numWaterIndices)++;
				waterIndices[*numWaterIndices] = trueNumVertices - 4;
				(*numWaterIndices)++;
				waterIndices[*numWaterIndices] = trueNumVertices - 2;
				(*numWaterIndices)++;
				waterIndices[*numWaterIndices] = trueNumVertices - 1;
				(*numWaterIndices)++;
			}
		}
		else {
			float texCoords[8];
			getTextureCoordinates(texCoords, m_blockIdToTextureNum[m_blocks[block] * 6 + textureNum]);
			for (short vertex = 0; vertex < 4; vertex++) {
				for (short element = 0; element < 3; element++) {
					vertices[*numVertices] = constants::CUBE_FACE_POSITIONS[firstPositionIndex + vertex * 3 + element] + blockPos[element];
					(*numVertices)++;
				}
				vertices[*numVertices] = texCoords[vertex * 2];
				(*numVertices)++;
				vertices[*numVertices] = texCoords[vertex * 2 + 1];
				(*numVertices)++;
				vertices[*numVertices] = (1.0f / 16.0f) * (getWorldSkyLight(neighbouringBlockPos) + 1);
				(*numVertices)++;
			}

			//ambient occlusion
			short blockType;
			for (unsigned char adjacentBlockToFace = 0; adjacentBlockToFace < 8; adjacentBlockToFace++) {
				int blockCoords[3];
				blockCoords[0] = block % constants::CHUNK_SIZE;
				blockCoords[1] = block / (constants::CHUNK_SIZE * constants::CHUNK_SIZE);
				blockCoords[2] = (block - blockCoords[1] * constants::CHUNK_SIZE * constants::CHUNK_SIZE) / constants::CHUNK_SIZE;
				blockCoords[0] += m_position[0] * constants::CHUNK_SIZE + m_neighbouringBlocksX[neighbouringBlockCoordsOffset] + m_adjacentBlocksToFaceOffestsX[firstAdjacentBlockIndex + adjacentBlockToFace];
				blockCoords[1] += m_position[1] * constants::CHUNK_SIZE + m_neighbouringBlocksY[neighbouringBlockCoordsOffset] + m_adjacentBlocksToFaceOffestsY[firstAdjacentBlockIndex + adjacentBlockToFace];
				blockCoords[2] += m_position[2] * constants::CHUNK_SIZE + m_neighbouringBlocksZ[neighbouringBlockCoordsOffset] + m_adjacentBlocksToFaceOffestsZ[firstAdjacentBlockIndex + adjacentBlockToFace];
				blockType = getWorldBlock(blockCoords);
				if (constants::castsShadows[blockType]) {
					if ((adjacentBlockToFace % 2) == 0) {
						vertices[*numVertices - (19 - adjacentBlockToFace / 2 * 6)] *= constants::shadowReceiveAmount[m_blocks[block]];
					}
					else {
						vertices[*numVertices - (19 - adjacentBlockToFace / 2 * 6)] *= constants::shadowReceiveAmount[m_blocks[block]];
						vertices[*numVertices - (19 - ((adjacentBlockToFace / 2 + 1) % 4) * 6)] *= constants::shadowReceiveAmount[m_blocks[block]];
					}
				}
			}

			//index buffer
			int trueNumVertices = *numVertices / 6;
			indices[*numIndices] = trueNumVertices - 4;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 3;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 2;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 4;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 2;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 1;
			(*numIndices)++;
		}
	} 
	else { //if the block is not a cube mesh
		//TODO:
		//make it so that it only adds the mesh if the mesh has not already been added (as only one "face" needs to be visible for them all to be visible
		float texCoords[8];
		short firstPositionIndex;

		for (unsigned char face = 0; face < 4; face++) {
			firstPositionIndex = face * 12;
			getTextureCoordinates(texCoords, m_blockIdToTextureNum[m_blocks[block] * 6 + face]);
			for (short vertex = 0; vertex < 4; vertex++) {
				for (short element = 0; element < 3; element++) {
					vertices[*numVertices] = constants::X_FACE_POSITIONS[firstPositionIndex + vertex * 3 + element] + blockPos[element];
					(*numVertices)++;
				}
				vertices[*numVertices] = texCoords[vertex * 2];
				(*numVertices)++;
				vertices[*numVertices] = texCoords[vertex * 2 + 1];
				(*numVertices)++;
				vertices[*numVertices] = (1.0f / 16.0f) * (getWorldSkyLight(neighbouringBlockPos) + 1);
				(*numVertices)++;
			}

			//TODO
			//lighting for X-shaped meshes

			//index buffer
			int trueNumVertices = *numVertices / 6;
			indices[*numIndices] = trueNumVertices - 4;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 3;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 2;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 4;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 2;
			(*numIndices)++;
			indices[*numIndices] = trueNumVertices - 1;
			(*numIndices)++;
		}
	}
}

void chunk::buildMesh(float* vertices, unsigned int* numVertices, unsigned int* indices, unsigned int* numIndices, float* waterVertices, unsigned int* numWaterVertices, unsigned int* waterIndices, unsigned int* numWaterIndices, unsigned int* neighbouringChunkIndices) {
	if (!m_skyLightUpToDate) {
		bool neighbouringChunksToRelight[6];
		s_checkingNeighbouringRelights.lock();
		bool neighbourBeingRelit = true;
		while (neighbourBeingRelit) {
			neighbourBeingRelit = false;
			for (unsigned int i = 0; i < 6; i++) {
				neighbourBeingRelit |= m_worldInfo.worldChunks[neighbouringChunkIndices[i]].skyBeingRelit();
			}
			this_thread::sleep_for(100us);
		}
		m_calculatingSkylight = true;
		clearSkyLight();
		calculateSkyLight(neighbouringChunkIndices, neighbouringChunksToRelight);
		m_calculatingSkylight = false;
	}

	//TODO:	make it so that chunks of a single block type get meshes without calling the new function
	unsigned char* tempBlocks = nullptr;
	if (m_singleBlockType) {
		tempBlocks = new unsigned char[constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
		for (unsigned int block = 0; block < (constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE); block++) {
			tempBlocks[block] = m_blocks[0];
		}
		unsigned char* temp = m_blocks;
		m_blocks = tempBlocks;
		tempBlocks = temp;
	}

	int blockPos[3];
	int neighbouringBlockPos[3];
	int blockNum = 0;
	for (blockPos[1] = m_position[1] * constants::CHUNK_SIZE; blockPos[1] < (m_position[1] + 1) * constants::CHUNK_SIZE; blockPos[1]++) {
		for (blockPos[2] = m_position[2] * constants::CHUNK_SIZE; blockPos[2] < (m_position[2] + 1) * constants::CHUNK_SIZE; blockPos[2]++) {
			for (blockPos[0] = m_position[0] * constants::CHUNK_SIZE; blockPos[0] < (m_position[0] + 1) * constants::CHUNK_SIZE; blockPos[0]++) {
				if (m_blocks[blockNum] == 0) {
					blockNum++;
					continue;
				}
				if (m_blocks[blockNum] == 4) {
					for (short neighbouringBlock = 0; neighbouringBlock < 6; neighbouringBlock++) {
						neighbouringBlockPos[0] = blockPos[0] + m_neighbouringBlocksX[neighbouringBlock];
						neighbouringBlockPos[1] = blockPos[1] + m_neighbouringBlocksY[neighbouringBlock];
						neighbouringBlockPos[2] = blockPos[2] + m_neighbouringBlocksZ[neighbouringBlock];
						if (getWorldBlock(neighbouringBlockPos) != 4) {
							addFaceToMesh(vertices, numVertices, indices, numIndices, waterVertices, numWaterVertices, waterIndices, numWaterIndices, blockNum, neighbouringBlock);
						}
					}
				}
				else {
					for (short neighbouringBlock = 0; neighbouringBlock < 6; neighbouringBlock++) {
						neighbouringBlockPos[0] = blockPos[0] + m_neighbouringBlocksX[neighbouringBlock];
						neighbouringBlockPos[1] = blockPos[1] + m_neighbouringBlocksY[neighbouringBlock];
						neighbouringBlockPos[2] = blockPos[2] + m_neighbouringBlocksZ[neighbouringBlock];
						if (constants::transparent[getWorldBlock(neighbouringBlockPos)]) {
							addFaceToMesh(vertices, numVertices, indices, numIndices, waterVertices, numWaterVertices, waterIndices, numWaterIndices, blockNum, neighbouringBlock);
						}
					}
				}
				blockNum++;
			}
		}
	}

	//TODO:	make it so that chunks of a single block type get meshes without calling the new function
	if (m_singleBlockType) {
		unsigned char* temp = m_blocks;
		m_blocks = tempBlocks;
		delete[] temp;
	}
}

void chunk::getTextureCoordinates(float* coords, short textureNum) {
	coords[0] = textureNum % 227 * 0.00439453125f + 0.000244140625f;
	coords[1] = 0.9956054688f - (textureNum / 227 * 0.00439453125f) + 0.000244140625f;
	coords[2] = coords[0] + 0.00390625f;
	coords[3] = coords[1];
	coords[4] = coords[2];
	coords[5] = coords[1] + 0.00390625f;
	coords[6] = coords[0];
	coords[7] = coords[5];
}

void chunk::getChunkPosition(int* coordinates) {
	for (char i = 0; i < 3; i++) {
		coordinates[i] = m_position[i];
	}
}

void chunk::unload() {
	inUse = false;

	delete[] m_blocks;
	delete[] m_skyLight;
	m_blocks = new unsigned char[0];
	m_skyLight = new unsigned char[0];
}

unsigned int chunk::getBlockNumber(unsigned int* blockCoords) {
	return blockCoords[0] + blockCoords[1] * constants::CHUNK_SIZE * constants::CHUNK_SIZE + blockCoords[2] * constants::CHUNK_SIZE;
}

void chunk::setBlock(unsigned int block, unsigned short blockType) {
	if (m_singleBlockType) {
		unsigned char* tempBlocks = new unsigned char[constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
		for (unsigned int block = 0; block < (constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE); block++) {
			tempBlocks[block] = m_blocks[0];
		}
		unsigned char* temp = m_blocks;
		m_blocks = tempBlocks;
		m_singleBlockType = false;
		delete[] temp;
	}
	m_blocks[block] = blockType;
	setSkyLight(block, 0 * !(constants::castsShadows[blockType]));
}

unsigned char chunk::getWorldBlock(int* blockCoords) {
	int chunkCoords[3];
	unsigned int blockPosInChunk[3];
	for (unsigned char i = 0; i < 3; i++) {
		chunkCoords[i] = floor(static_cast<float>(blockCoords[i]) / constants::CHUNK_SIZE);
		blockPosInChunk[i] = blockCoords[i] - chunkCoords[i] * constants::CHUNK_SIZE;
	}

	//get the chunk number
	int adjustedChunkCoords[3];
	for (unsigned char i = 0; i < 3; i++) {
		adjustedChunkCoords[i] = chunkCoords[i] - m_worldInfo.playerChunkPosition[i] + m_worldInfo.renderDistance;
	}

	unsigned int chunkNumber = adjustedChunkCoords[1] * m_worldInfo.renderDiameter * m_worldInfo.renderDiameter;
	chunkNumber += adjustedChunkCoords[2] * m_worldInfo.renderDiameter;
	chunkNumber += adjustedChunkCoords[0];


	unsigned int blockNumber = m_worldInfo.worldChunks[m_worldInfo.chunkArrayIndices[chunkNumber]].getBlockNumber(blockPosInChunk);

	return m_worldInfo.worldChunks[m_worldInfo.chunkArrayIndices[chunkNumber]].getBlock(blockNumber);
}

unsigned char chunk::getWorldSkyLight(int* blockCoords) {
	int chunkCoords[3];
	unsigned int blockPosInChunk[3];
	for (unsigned char i = 0; i < 3; i++) {
		chunkCoords[i] = floor(static_cast<float>(blockCoords[i]) / constants::CHUNK_SIZE);
		blockPosInChunk[i] = blockCoords[i] - chunkCoords[i] * constants::CHUNK_SIZE;
	}

	//get the chunk number
	int adjustedChunkCoords[3];
	for (unsigned char i = 0; i < 3; i++) {
		adjustedChunkCoords[i] = chunkCoords[i] - m_worldInfo.playerChunkPosition[i] + m_worldInfo.renderDistance;
	}

	unsigned int chunkNumber = adjustedChunkCoords[1] * m_worldInfo.renderDiameter * m_worldInfo.renderDiameter;
	chunkNumber += adjustedChunkCoords[2] * m_worldInfo.renderDiameter;
	chunkNumber += adjustedChunkCoords[0];


	unsigned int blockNumber = m_worldInfo.worldChunks[m_worldInfo.chunkArrayIndices[chunkNumber]].getBlockNumber(blockPosInChunk);

	return m_worldInfo.worldChunks[m_worldInfo.chunkArrayIndices[chunkNumber]].getSkyLight(blockNumber);
}

float chunk::getTerrainHeight(float noiseValue, const float* noisePoints, const float* terrainPoints) {
	int i = 0;
	while (noisePoints[i + 1] < noiseValue) {
		i++;
	}
	float frac;
	float denominator = noisePoints[i + 1] - noisePoints[i];
	if (denominator != 0.0) {
		frac = (noiseValue - noisePoints[i]) / denominator;
	}
	else {
		frac = 0;
	}
	return terrainPoints[i] + frac * (terrainPoints[i + 1] - terrainPoints[i]);
}

void chunk::clearSkyLight() {
	//reset all sky light values in the chunk to 0
	for (unsigned int blockNum = 0; blockNum < ((constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE + 1) / 2); blockNum++) {
		m_skyLight[blockNum] = 0;
	}
}

void chunk::calculateSkyLight(unsigned int* neighbouringChunkIndices, bool* neighbouringChunksToBeRelit) {
	if (!m_calculatingSkylight) {
		s_checkingNeighbouringRelights.lock();
		bool neighbourBeingRelit = true;
		while (neighbourBeingRelit) {
			neighbourBeingRelit = false;
			for (unsigned int i = 0; i < 6; i++) {
				neighbourBeingRelit |= m_worldInfo.worldChunks[neighbouringChunkIndices[i]].skyBeingRelit();
			}
			this_thread::sleep_for(100us);
		}
	}
	s_checkingNeighbouringRelights.unlock();

	(*m_worldInfo.numRelights)++;

	//TODO:	make it so that chunks of a single block type get meshes without calling the new function
	unsigned char* tempBlocks = nullptr;
	if (m_singleBlockType) {
		tempBlocks = new unsigned char[constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE];
		for (unsigned int block = 0; block < (constants::CHUNK_SIZE * constants::CHUNK_SIZE * constants::CHUNK_SIZE); block++) {
			tempBlocks[block] = m_blocks[0];
		}
		unsigned char* temp = m_blocks;
		m_blocks = tempBlocks;
		tempBlocks = temp;
	}

	queue<unsigned int> lightQueue;
	//add the light values from chunk borders to the light queue
	unsigned int blockNum = 0;
	for (unsigned int z = 0; z < constants::CHUNK_SIZE; z++) {
		for (unsigned int x = 0; x < constants::CHUNK_SIZE; x++) {
			char currentskyLight = getSkyLight(blockNum);
			char neighbouringskyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[0]].getSkyLight(
				blockNum + constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) - 1;
			bool newHighestSkyLight = (currentskyLight < neighbouringskyLight) * !constants::castsShadows[m_blocks[blockNum]];
			if (newHighestSkyLight) {
				setSkyLight(blockNum, neighbouringskyLight);
				lightQueue.push(blockNum);
			}
			blockNum++;
		}
	}
	blockNum = 0;
	for (unsigned int y = 0; y < constants::CHUNK_SIZE; y++) {
		for (unsigned int x = 0; x < constants::CHUNK_SIZE; x++) {
			char currentskyLight = getSkyLight(blockNum);
			char neighbouringskyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[1]].getSkyLight(
				blockNum + constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) - 1;
			bool newHighestSkyLight = (currentskyLight < neighbouringskyLight) * !constants::castsShadows[m_blocks[blockNum]];
			if (newHighestSkyLight) {
				setSkyLight(blockNum, neighbouringskyLight);
				lightQueue.push(blockNum);
			}
			blockNum++;
		}
		blockNum += constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1);
	}
	blockNum = 0;
	for (unsigned int y = 0; y < constants::CHUNK_SIZE; y++) {
		for (unsigned int z = 0; z < constants::CHUNK_SIZE; z++) {
			char currentskyLight = getSkyLight(blockNum);
			char neighbouringskyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[2]].getSkyLight(
				blockNum + constants::CHUNK_SIZE - 1) - 1;
			bool newHighestSkyLight = (currentskyLight < neighbouringskyLight) * !constants::castsShadows[m_blocks[blockNum]];
			if (newHighestSkyLight) {
				setSkyLight(blockNum, neighbouringskyLight);
				lightQueue.push(blockNum);
			}
			blockNum += constants::CHUNK_SIZE;
		}
	}
	blockNum = constants::CHUNK_SIZE - 1;
	for (unsigned int y = 0; y < constants::CHUNK_SIZE; y++) {
		for (unsigned int z = 0; z < constants::CHUNK_SIZE; z++) {
			char currentskyLight = getSkyLight(blockNum);
			char neighbouringskyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[3]].getSkyLight(
				blockNum - constants::CHUNK_SIZE + 1) - 1;
			bool newHighestSkyLight = (currentskyLight < neighbouringskyLight) * !constants::castsShadows[m_blocks[blockNum]];
			if (newHighestSkyLight) {
				setSkyLight(blockNum, neighbouringskyLight);
				lightQueue.push(blockNum);
			}
			blockNum += constants::CHUNK_SIZE;
		}
	}
	blockNum = constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1);
	for (unsigned int y = 0; y < constants::CHUNK_SIZE; y++) {
		for (unsigned int x = 0; x < constants::CHUNK_SIZE; x++) {
			char currentskyLight = getSkyLight(blockNum);
			char neighbouringskyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[4]].getSkyLight(
				blockNum - constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) - 1;
			bool newHighestSkyLight = (currentskyLight < neighbouringskyLight) * !constants::castsShadows[m_blocks[blockNum]];
			if (newHighestSkyLight) {
				setSkyLight(blockNum, neighbouringskyLight);
				lightQueue.push(blockNum);
			}
			blockNum++;
		}
		blockNum += constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1);
	}
	blockNum = constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1);
	for (unsigned int z = 0; z < constants::CHUNK_SIZE; z++) {
		for (unsigned int x = 0; x < constants::CHUNK_SIZE; x++) {
			char currentskyLight = getSkyLight(blockNum);
			char neighbouringskyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[5]].getSkyLight(
				blockNum - constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) - 1;
			//propogate direct skylight down without reducing its intensity
			neighbouringskyLight += (neighbouringskyLight == 14)
				* !(constants::dimsLight[m_worldInfo.worldChunks[neighbouringChunkIndices[5]].getBlock(
				blockNum - constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))]);
			bool newHighestSkyLight = (currentskyLight < neighbouringskyLight) * !constants::castsShadows[m_blocks[blockNum]];
			if (newHighestSkyLight) {
				setSkyLight(blockNum, neighbouringskyLight);
				lightQueue.push(blockNum);
			}
			blockNum++;
		}
	}
	//propogate the light values to the neighbouring blocks in the chunk
	for (char i = 0; i < 6; i++) {
		neighbouringChunksToBeRelit[i] = false;
	}
	while (!lightQueue.empty()) {
		unsigned int blockNum = lightQueue.front();
		unsigned char skyLight = getSkyLight(blockNum) - 1;
		lightQueue.pop();
		if (blockNum < (constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))) {
			unsigned int neighbouringBlockNum = blockNum + m_neighbouringBlocks[5];
			unsigned char neighbouringskyLight = getSkyLight(neighbouringBlockNum);
			bool newHighestSkyLight = (neighbouringskyLight < skyLight) * !constants::castsShadows[m_blocks[neighbouringBlockNum]];
			if (newHighestSkyLight) {
				setSkyLight(neighbouringBlockNum, skyLight);
				lightQueue.push(neighbouringBlockNum);
			}
		}
		else {
			bool transparrent = constants::transparent[m_worldInfo.worldChunks[neighbouringChunkIndices[5]].getBlock(
				blockNum - constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))];
			bool newHighestSkyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[5]].getSkyLight(
				blockNum - constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) < skyLight;
			if (transparrent && newHighestSkyLight) {
				neighbouringChunksToBeRelit[5] = true;
			}
		}
		if ((blockNum % (constants::CHUNK_SIZE * constants::CHUNK_SIZE)) < (constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))) {
			unsigned int neighbouringBlockNum = blockNum + m_neighbouringBlocks[4];
			unsigned char neighbouringskyLight = getSkyLight(neighbouringBlockNum);
			bool newHighestSkyLight = (neighbouringskyLight < skyLight) * !constants::castsShadows[m_blocks[neighbouringBlockNum]];
			if (newHighestSkyLight) {
				setSkyLight(neighbouringBlockNum, skyLight);
				lightQueue.push(neighbouringBlockNum);
			}
		}
		else {
			bool transparrent = constants::transparent[m_worldInfo.worldChunks[neighbouringChunkIndices[4]].getBlock(
				blockNum - constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))];
			bool newHighestSkyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[4]].getSkyLight(
				blockNum - constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) < skyLight;
			if (transparrent && newHighestSkyLight) {
				neighbouringChunksToBeRelit[4] = true;
			}
		}
		if ((blockNum % constants::CHUNK_SIZE) < (constants::CHUNK_SIZE - 1)) {
			unsigned int neighbouringBlockNum = blockNum + m_neighbouringBlocks[3];
			unsigned char neighbouringskyLight = getSkyLight(neighbouringBlockNum);
			bool newHighestSkyLight = (neighbouringskyLight < skyLight) * !constants::castsShadows[m_blocks[neighbouringBlockNum]];
			if (newHighestSkyLight) {
				setSkyLight(neighbouringBlockNum, skyLight);
				lightQueue.push(neighbouringBlockNum);
			}
		}
		else {
			bool transparrent = constants::transparent[m_worldInfo.worldChunks[neighbouringChunkIndices[3]].getBlock(
				blockNum - (constants::CHUNK_SIZE - 1))];
			bool newHighestSkyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[3]].getSkyLight(
				blockNum - (constants::CHUNK_SIZE - 1)) < skyLight;
			if (transparrent && newHighestSkyLight) {
				neighbouringChunksToBeRelit[3] = true;
			}
		}
		if ((blockNum % constants::CHUNK_SIZE) >= 1) {
			unsigned int neighbouringBlockNum = blockNum + m_neighbouringBlocks[2];
			unsigned char neighbouringskyLight = getSkyLight(neighbouringBlockNum);
			bool newHighestSkyLight = (neighbouringskyLight < skyLight) * !constants::castsShadows[m_blocks[neighbouringBlockNum]];
			if (newHighestSkyLight) {
				setSkyLight(neighbouringBlockNum, skyLight);
				lightQueue.push(neighbouringBlockNum);
			}
		}
		else {
			bool transparrent = constants::transparent[m_worldInfo.worldChunks[neighbouringChunkIndices[2]].getBlock(
				blockNum + (constants::CHUNK_SIZE - 1))];
			bool newHighestSkyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[2]].getSkyLight(
				blockNum + (constants::CHUNK_SIZE - 1)) < skyLight;
			if (transparrent && newHighestSkyLight) {
				neighbouringChunksToBeRelit[2] = true;
			}
		}
		if ((blockNum % (constants::CHUNK_SIZE * constants::CHUNK_SIZE)) >= constants::CHUNK_SIZE) {
			unsigned int neighbouringBlockNum = blockNum + m_neighbouringBlocks[1];
			unsigned char neighbouringskyLight = getSkyLight(neighbouringBlockNum);
			bool newHighestSkyLight = (neighbouringskyLight < skyLight) * !constants::castsShadows[m_blocks[neighbouringBlockNum]];
			if (newHighestSkyLight) {
				setSkyLight(neighbouringBlockNum, skyLight);
				lightQueue.push(neighbouringBlockNum);
			}
		}
		else {
			bool transparrent = constants::transparent[m_worldInfo.worldChunks[neighbouringChunkIndices[1]].getBlock(
				blockNum + constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))];
			bool newHighestSkyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[1]].getSkyLight(
				blockNum + constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) < skyLight;
			if (transparrent && newHighestSkyLight) {
				neighbouringChunksToBeRelit[1] = true;
			}
		}
		if (blockNum >= (constants::CHUNK_SIZE * constants::CHUNK_SIZE)) {
			unsigned int neighbouringBlockNum = blockNum + m_neighbouringBlocks[0];
			skyLight += (skyLight == 14) * !(constants::dimsLight[m_blocks[neighbouringBlockNum]]);
			unsigned char neighbouringskyLight = getSkyLight(neighbouringBlockNum);
			bool newHighestSkyLight = (neighbouringskyLight < skyLight) * !constants::castsShadows[m_blocks[neighbouringBlockNum]];
			if (newHighestSkyLight) {
				setSkyLight(neighbouringBlockNum, skyLight);
				lightQueue.push(neighbouringBlockNum);
			}
		}
		else {
			bool dimsLight = constants::dimsLight[m_worldInfo.worldChunks[neighbouringChunkIndices[0]].getBlock(
				blockNum + constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))];
			skyLight += (skyLight == 14) * !(dimsLight);
			bool transparrent = constants::transparent[m_worldInfo.worldChunks[neighbouringChunkIndices[0]].getBlock(
				blockNum + constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1))];
			bool newHighestSkyLight = m_worldInfo.worldChunks[neighbouringChunkIndices[0]].getSkyLight(
				blockNum + constants::CHUNK_SIZE * constants::CHUNK_SIZE * (constants::CHUNK_SIZE - 1)) < skyLight;
			if (transparrent && newHighestSkyLight) {
				neighbouringChunksToBeRelit[0] = true;
			}
		}
	}

	//TODO:	make it so that chunks of a single block type get meshes without calling the new function
	if (m_singleBlockType) {
		unsigned char* temp = m_blocks;
		m_blocks = tempBlocks;
		delete[] temp;
	}

	
	(*m_worldInfo.numRelights)--;
	m_skyLightUpToDate = true;
	m_calculatingSkylight = false;
	// lock release
	lock_guard<mutex> lock(m_accessingSkylightMtx);
	m_accessingSkylightCV.notify_all();
}