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

#include <unordered_set>

#include "core/chunk.h"
#include "core/position.h"

class ServerPlayer {
private:
	unsigned short m_renderDistance;
	unsigned short m_renderDiameter;
    float m_minUnloadedChunkDistance;
    unsigned int m_numChunks;
    double m_position[3];
    Position* m_unloadedChunks;
    int* m_playerChunkPosition;
    int m_nextUnloadedChunk;
    std::unordered_set<Position> m_loadedChunks;

    void initChunkPositions();
    void initNumChunks();
public:
    ServerPlayer(double* position, unsigned short renderDistance);
    void updatePlayerPos(double* position);
    bool allChunksLoaded();
    void getNextChunkCoords(int* chunkCoords);
};