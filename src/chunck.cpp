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

//working with chunk
//by the way Chunk& is not holding the address but the alias
void Chunk::AddCubeToChunk(ChunkTemp& chunk, float x, float y, float z) {
    // Define the 8 corners of a cube (relative to 0,0,0) then add x,y,z
    // We use your 0.5f size convention
    std::vector<GLfloat> vertices = {
	-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	-0.5f+x,  0.5f+y, -0.5f+z,  0.0f, 1.0f,  1.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, 0.0f,  1.0f,

	// FRONT FACE (Grass Side -> 1.0)
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y,  0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 1.0f,  1.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 1.0f,  1.0f,
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,

	// LEFT FACE (Grass Side -> 1.0)
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
	-0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,

	// RIGHT FACE (Grass Side -> 1.0)
	 0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  1.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 0.0f,  1.0f,
	 0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  1.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 1.0f,  1.0f,

	// BOTTOM FACE (Dirt -> 0.0)
	-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, 1.0f,  0.0f,
	 0.5f+x, -0.5f+y, -0.5f+z,  1.0f, 1.0f,  0.0f,
	 0.5f+x, -0.5f+y,  0.5f+z,  1.0f, 0.0f,  0.0f,
	 0.5f+x, -0.5f+y,  0.5f+z,  1.0f, 0.0f,  0.0f,
	-0.5f+x, -0.5f+y,  0.5f+z,  0.0f, 0.0f,  0.0f,
	-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, 1.0f,  0.0f,

	// TOP FACE (Grass Top -> 2.0)
	-0.5f+x,  0.5f+y, -0.5f+z,  0.0f, 1.0f,  2.0f,
	 0.5f+x,  0.5f+y, -0.5f+z,  1.0f, 1.0f,  2.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 0.0f,  2.0f,
	 0.5f+x,  0.5f+y,  0.5f+z,  1.0f, 0.0f,  2.0f,
	-0.5f+x,  0.5f+y,  0.5f+z,  0.0f, 0.0f,  2.0f,
	-0.5f+x,  0.5f+y, -0.5f+z,  0.0f, 1.0f,  2.0f
    };
    //you are basically pushing 180 floats inisde meshData
    for (GLfloat v : vertices) {
        chunk.meshDataTemp.push_back(v);
    }
}

std::vector<GLfloat> Chunk::draw(int chunkX, int chunkZ){
    ChunkTemp chunk;
    Headers headers;
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            
            // 1. Convert world position to "Noise Space"
            // We divide by a 'frequency' so the hills aren't too steep
            float noiseX = (x + chunkX * 16);
            float noiseZ = (z + chunkZ * 16);
            
            // 2. Run your Perlin math!
            float perlinValue = headers.mTerrain.perlin(noiseX * 0.05, noiseZ *0.05); 
            float heightDecider = headers.mTerrain.perlin(noiseX * 0.012, noiseZ *0.012); 
            
            // 3. Map -1.0...1.0 to a height (e.g., 0 to 20 blocks)
            int height = ((int)((perlinValue + 1.0f) * 84.0f*heightDecider))+40;
            
            // 4. Place the cubes!
            for (int y = 0; y < height; y++) {
		Chunk::AddCubeToChunk(chunk, x, y, z);
            }
        }
    }
    return chunk.meshDataTemp;
}
