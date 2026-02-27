#pragma once
#include <glm/vec2.hpp>
#include <glad/glad.h>

class Terrain{
    public:
    Terrain();
    float perlin(float noiseX, float noiseZ);

    private:
    glm::vec2 randomGradiantVector(glm::vec2);
};
