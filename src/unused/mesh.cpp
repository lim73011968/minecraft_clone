#include <iostream>
#include <vector>
#include "mesh.hpp"
#include "glad/glad.h"
#include "stb_image.h"
#include <fstream>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


Mesh::Mesh(){

};

struct App{
    GLuint mGraphicsPipelineShaderProgram = 0;
    GLuint mtexture;
};

struct Transform{
    float x, y, z;
};

struct Geometry3D{
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint IBO = 0;
};

struct Mesh3D{
    Geometry3D* geometry;
    Transform mtransform;
    GLuint mPipeline = 0;
};
App gApp;
Mesh3D gMesh;

void MeshSetPipeline(Mesh3D* mesh, GLuint pipeline){
    mesh->mPipeline = pipeline;
}

Geometry3D MeshDataVertexSpecfication(){
    Geometry3D geo;
    const std::vector<GLfloat> vertexData{
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    //vao intiallize
    glGenVertexArrays(1,&geo.VAO);
    glBindVertexArray(geo.VAO);
    //vbo
    glGenBuffers(1, &geo.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geo.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    //ibo
    const std::vector<GLuint> indexBufferData {2, 0, 1, 3, 2, 1};
    glGenBuffers(1, &geo.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);
    //connecting to VAO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);

    //color

    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat)*3));
    
    //by the way this is yo texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(3 * sizeof(GLfloat)));
    
    //now disable
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

    return geo;
}

//working with texture
void LoadTexture(){
    glGenTextures(1, &gApp.mtexture);
    //generate texture
    glBindTexture(GL_TEXTURE_2D, gApp.mtexture);
    //bind it like VAO and VBO

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    //repates the texture image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // S and T is basically S and Y for the textrure
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //process where you filter the pixel bases on how far and near you are 
    //more info look at Texture section opengl learn
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../texture/cobblestone.jpg", &width, &height, &nrChannels, 0); 
    //this loads the images (the location, gets the width, height, the rgb(color) can be anything, how many channels you want??
    //what is 0 at the end??? --> rgb has 3 channels and rgba has 4 channels how many channels you want the STB to bring???
    if (data)
    {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//the texture target, the level of mipmap(0 is original 1 half size ....), width, height, related with border just keep it one, the formate of image, the data type of your image, the actual data's pinter
	glGenerateMipmap(GL_TEXTURE_2D);
	//create mipmap automatically
    }
    else
    {
	std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

//working with shader
std::string LoadShaderAsString(const std::string& filename){
    //gets the whole string from both glsl file
    std::string result = "";
    std::string line = "";
    std::ifstream myFile(filename.c_str());
    if(myFile.is_open()){
	while (std::getline(myFile, line)) {
	    result += line + '\n';
	}
	myFile.close();
    }
    return result;
}

GLuint CompileShader(GLuint type, const std::string& source){
    //compiles your shader
    GLuint shaderObject;
    if(type == GL_VERTEX_SHADER){
	shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(type == GL_FRAGMENT_SHADER){
	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);



    //some error for your shader
    int result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
	int length;
	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
	char* errorMessage = new char[length];
	glGetShaderInfoLog(shaderObject, length, &length, errorMessage);

	if (type == GL_VERTEX_SHADER){
	    std::cout << "Vertex Shader error:" << errorMessage << std::endl;
	} else if (type == GL_FRAGMENT_SHADER) {
	    std::cout << "Fragment Shader error: " << errorMessage << std::endl;
	}
	delete [] errorMessage;
    }
    //glDeleteShader(shaderObject);

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexshadersource, const std::string& fragmentshadersource){
    //create shader program...
    GLuint programObject = glCreateProgram();
    GLuint VertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    GLuint FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);
    glAttachShader(programObject, VertexShader);
    glAttachShader(programObject, FragmentShader);
    glLinkProgram(programObject);
    glValidateProgram(programObject);
    return programObject;
}

void CreateGraphicsPipeline(){
    std::string vertexShaderSource = LoadShaderAsString("./shader/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shader/frag.glsl");
    gApp.mGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void PrepareWorld(App* app){
    //use shader program
    glUseProgram(app->mGraphicsPipelineShaderProgram);

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)app->mScreenWidth/(float)app->mScreenHeight, 0.1f, 100.0f);
    GLint u_Persp = glGetUniformLocation(app->mGraphicsPipelineShaderProgram, "u_Perspective");
    glUniformMatrix4fv(u_Persp, 1, GL_FALSE, &perspective[0][0]);

    glm::mat4 view = app->mCamera.GetViewMatrix();
    GLint u_View = glGetUniformLocation(app->mGraphicsPipelineShaderProgram, "u_ViewMatrix");
    glUniformMatrix4fv(u_View, 1, GL_FALSE, &view[0][0]);
}

//actually rendering your Mesh
void RenderMesh(Mesh3D* mesh, App* app) {
    //deciding your position
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(mesh->mtransform.x, mesh->mtransform.y, mesh->mtransform.z));

    //rotation
    //float time = (float)SDL_GetTicks() / 1000.0f;
    //model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    
    //tells the Vertex Shader or GPU to move to world space
    GLint u_Model = glGetUniformLocation(mesh->pipeline, "u_ModelMatrix");
    glUniformMatrix4fv(u_Model, 1, GL_FALSE, &model[0][0]);

    glBindTexture(GL_TEXTURE_2D, app->mtexture);
    //recall VAO to get the texture data again
    glBindVertexArray(mesh->geometry->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void Mesh::SingleMeshDraw(){
    MeshSetPipeline(&gMesh, gApp.mGraphicsPipelineShaderProgram);
}
