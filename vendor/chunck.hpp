#pragma once
#include <glad/glad.h>

#include <vector>
class Chunk{
    public:
    Chunk();
    std::vector<GLfloat> draw(int chunkX, int chunkZ);

    private:
    struct ChunkTemp;
    struct Headers;
    void AddCubeToChunk(ChunkTemp& chunk, float x, float y, float z);
};
