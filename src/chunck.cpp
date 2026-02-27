#include "chunck.hpp"
#include "terrain.hpp"
#include <glad/glad.h>
#include <vector>

Chunk::Chunk(){
    
};

struct Chunk::ChunkTemp {
    std::vector<GLfloat> meshDataTemp;
};

struct Chunk::Headers {
    Terrain mTerrain;
};

//by the way Chunk& is not holding the address but the alias
void Chunk::AddCubeTop(ChunkTemp& chunk, float x, float y, float z){
    std::vector<GLfloat> vertices = {
	-0.5f+x,  0.5f+y, -0.5f+z,  0.0f, 1.0f,  2.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  2.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 0.0f,  2.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 0.0f,  2.0f,
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 0.0f,  2.0f,
	-0.5f+x,  0.5f+y, -0.5f+z,  0.0f, 1.0f,  2.0f,
    };
    for (GLfloat v : vertices) {
        chunk.meshDataTemp.push_back(v);
    }
}

void Chunk::AddCubeRight(ChunkTemp& chunk, float x, float y, float z){
    std::vector<GLfloat> vertices = {
	 0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
    };
    for (GLfloat v : vertices) {
        chunk.meshDataTemp.push_back(v);
    }
}

void Chunk::AddCubeLeft(ChunkTemp& chunk, float x, float y, float z){
    std::vector<GLfloat> vertices = {
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
	-0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
    };
    for (GLfloat v : vertices) {
        chunk.meshDataTemp.push_back(v);
    }
}

void Chunk::AddCubeFront(ChunkTemp& chunk, float x, float y, float z){
    std::vector<GLfloat> vertices = {
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y,  0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 1.0f,  1.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 1.0f,  1.0f,
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
    };
    for (GLfloat v : vertices) {
        chunk.meshDataTemp.push_back(v);
    }
}

void Chunk::AddCubeBack(ChunkTemp& chunk, float x, float y, float z){
    std::vector<GLfloat> vertices = {
	-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	-0.5f+x,  0.5f+y, -0.5f+z,  0.0f, 1.0f,  1.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, 0.0f,  1.0f,
    };
    for (GLfloat v : vertices) {
        chunk.meshDataTemp.push_back(v);
    }
}

std::vector<GLfloat> Chunk::draw(int chunkX, int chunkZ) {
    ChunkTemp chunk;
    Headers headers;
    
    //chunks aare 18*18 becuase we have to also decide if we are going to draw the mesh dependiong on the next chunk over
    int heights[18][18]; 
    //we have to set all the cordinates and the location first cuase it's gonna crash cuase there is no cordinates when searching the neighbhors
    for (int x = -1; x < 17; x++) {
        for (int z = -1; z < 17; z++) {
            float noiseX = (x + chunkX * 16);
            float noiseZ = (z + chunkZ * 16);
            
            float perlinValue = headers.mTerrain.perlin(noiseX * 0.05, noiseZ * 0.05); 
            float heightDecider = headers.mTerrain.perlin(noiseX * 0.012, noiseZ * 0.012); 
            
            heights[x + 1][z + 1] = ((int)((perlinValue + 1.0f) * 84.0f * heightDecider)) + 40;
        }
    }

    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            int currentHeight = heights[x + 1][z + 1];
            
            for (int y = 0; y < currentHeight; y++) {
                if (y == currentHeight - 1) {
                    AddCubeTop(chunk, x, y, z);
                }
                if (y >= heights[x + 2][z + 1]) AddCubeRight(chunk, x, y, z); 
                if (y >= heights[x][z + 1])     AddCubeLeft(chunk, x, y, z);  
                if (y >= heights[x + 1][z + 2]) AddCubeFront(chunk, x, y, z); 
                if (y >= heights[x + 1][z])     AddCubeBack(chunk, x, y, z);  
            }
        }
    }
    return chunk.meshDataTemp;
}
