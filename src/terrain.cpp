#include "terrain.hpp"
#include "glm/common.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <glm/vec2.hpp>

Terrain::Terrain(){
}

glm::vec2 Terrain::randomGradiantVector(glm::vec2 gridEdge){
    gridEdge += 0.01;
    //dot product is not totally random but it still gives unique value for both x and y
    float randomVectorX = glm::dot(gridEdge, glm::vec2(123.4, 234.5));
    float randomVectorY = glm::dot(gridEdge, glm::vec2(234.5, 345.6));
    glm::vec2 gradient = glm::vec2(randomVectorX, randomVectorY);
    //puttin sin inorder to make it between 1.0, 1.0
    gradient = glm::sin(gradient);
    return gradient;
}

float Terrain::perlin(float noiseX, float noiseZ){
    glm::vec2 amoutOfChunk = glm::vec2(noiseX,noiseZ);
    //the grid size its going to be 4 x 4 which means 16 chunks
    //for now
    //the ID or the floor ex --> are we in the floor 1 or floor 2
    glm::vec2 gridID = glm::floor(amoutOfChunk);
    //the exact position ex--> if gives u 0.34 than you are in that position in the box
    glm::vec2 gridSpecification = glm::fract(amoutOfChunk);

    //this is the EDGE cordinate of each box
    glm::vec2 bottomLeft = gridID + glm::vec2(0.0f, 0.0f);
    glm::vec2 bottomRight = gridID + glm::vec2(1.0f, 0.0f);
    glm::vec2 topLeft = gridID + glm::vec2(0.0f, 1.0f);
    glm::vec2 topRight = gridID + glm::vec2(1.0f, 1.0f);
    
    //this is the random vectors for the edges!!!
    glm::vec2 randomGradientBottomLeft = Terrain::randomGradiantVector(bottomLeft);
    glm::vec2 randomGradientBottomRight = Terrain::randomGradiantVector(bottomRight);
    glm::vec2 randomGradientTopLeft = Terrain::randomGradiantVector(topLeft);
    glm::vec2 randomGradientTopRight = Terrain::randomGradiantVector(topRight);

    //finidng the distance of current pixel in the box from the edges
    glm::vec2 distanceFromPixelBottomLeft = gridSpecification - glm::vec2(0.0f, 0.0f);
    glm::vec2 distanceFromPixelBottomRight = gridSpecification - glm::vec2(1.0f, 0.0f);
    glm::vec2 distanceFromPixelTopLeft = gridSpecification - glm::vec2(0.0f, 1.0f);
    glm::vec2 distanceFromPixelTopRight = gridSpecification - glm::vec2(1.0f, 1.0f);

    //the dot prodcut of the distance and the random vectors
    float dotBottomLeft = glm::dot(randomGradientBottomLeft, distanceFromPixelBottomLeft);
    float dotBottomRight = glm::dot(randomGradientBottomRight, distanceFromPixelBottomRight);
    float dotTopLeft = glm::dot(randomGradientTopLeft, distanceFromPixelTopLeft);
    float dotTopRight = glm::dot(randomGradientTopRight, distanceFromPixelTopRight);

    //liner interpolation
    float bottom = glm::mix(dotBottomLeft, dotBottomRight, gridSpecification.x);
    float top = glm::mix(dotTopLeft, dotTopRight, gridSpecification.x);
    float perlin = glm::mix(bottom, top, gridSpecification.y);
    return perlin;
}
