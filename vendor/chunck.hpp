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

    void AddCubeTop(ChunkTemp& chunk, float x, float y, float z);
    void AddCubeFront(ChunkTemp& chunk, float x, float y, float z);
    void AddCubeBack(ChunkTemp& chunk, float x, float y, float z);
    void AddCubeLeft(ChunkTemp& chunk, float x, float y, float z);
    void AddCubeRight(ChunkTemp& chunk, float x, float y, float z);
};
