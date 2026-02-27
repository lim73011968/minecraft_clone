#include "glm/ext/matrix_float4x4.hpp"
#include <iostream>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/vector_float3.hpp"
#include "stb_image.h"
#include <map>

#include "camera.hpp"
#include "chunck.hpp"

struct LocationMesh{
    int x,z;
    bool operator!=(const LocationMesh& other) const {
        return x != other.x || z != other.z;
    }
};
struct App{
    int mScreenHeight = 800;
    int mScreenWidth = 800;
    SDL_Window* mGraphicsApplicationWindow = nullptr;

    //shaderProgram
    SDL_GLContext mOpenGLContext= nullptr;

    bool mQuit = false;
    //things for our initialze program for SDL3!!!
    GLuint mGraphicsPipelineShaderProgram = 0;

    unsigned int mtexture_cobblestone;
    unsigned int mtexture_dirt;
    unsigned int mtexture_grassblock;
    unsigned int mtexture_grassblock_top;

    Camera mCamera;
    Chunk mChunk;
    LocationMesh mLocationMesh;
};
struct Transform{
    float x,y,z;
};
struct Geometry3D {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint IBO = 0;
    int vertexCount = 0;
};
struct Mesh3D{

    Geometry3D geometry;
    //uniorm offsets
    float m_uOffset = -5.0f;
    Transform mtransform;
    //pipeline used with the Mesh
    GLuint pipeline = 0;
};

struct ChunkVectors {
    std::vector<GLfloat> meshData;
};

//------------------------globals------------------------
App gApp;
Geometry3D geo;
//this holds the location of your meshes
ChunkVectors gChunkVector;
//this just holds the whole meshes(cause you will be producing alot of meshes)
std::map<std::pair<int, int>,Mesh3D> gSceneMeshes;

//error checker
static void GLClearAllError(){
    while (glGetError() != GL_NO_ERROR) {
    }
}
static bool GLCheckErrorStatus(const char* function, int line){
    while(GLenum error = glGetError()){
	std::cout <<"OpenGL Error:" << error
		  << "\tLine: " << line
		  << "\tfunction: " <<function <<std::endl;
	return true;
    }
    return false;
}
#define GLCheck(x) GLClearAllError(); x; GLCheckErrorStatus(#x, __LINE__);

void MeshSetPipeline(Mesh3D* mesh, GLuint pipeline){
    mesh->pipeline = pipeline;
}

//Returns actual VBO data how they are going to be used (VAO) "the actual memeory location"
Geometry3D MeshDataVertexSpecfication(ChunkVectors& chunk){
    //becuase this is local variable only the return value will be saved
    Geometry3D geo;
    //vao intiallize
    glGenVertexArrays(1,&geo.VAO);
    glBindVertexArray(geo.VAO);
    //vbo
    glGenBuffers(1, &geo.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geo.VBO);
    glBufferData(GL_ARRAY_BUFFER, chunk.meshData.size() * sizeof(GLfloat), chunk.meshData.data(), GL_STATIC_DRAW);

    //ibo
    const std::vector<GLuint> indexBufferData {2, 0, 1, 3, 2, 1};
    glGenBuffers(1, &geo.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);

    //connecting to VAO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);

    //color

    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat)*3));
    
    //by the way this is yo texture placement
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(5 * sizeof(GLfloat)));

    geo.vertexCount = (int)chunk.meshData.size() / 6;
    //now disable
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

    return geo;
}
void LoadTexture() {
    int width, height, nrChannels;
    unsigned char* data;

    // --- 1. COBBLESTONE ---
    glGenTextures(1, &gApp.mtexture_cobblestone);
    glBindTexture(GL_TEXTURE_2D, gApp.mtexture_cobblestone);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("./texture/cobblestone.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    // --- 2. DIRT ---
    glGenTextures(1, &gApp.mtexture_dirt);
    glBindTexture(GL_TEXTURE_2D, gApp.mtexture_dirt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("./texture/Dirt_Block.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    stbi_set_flip_vertically_on_load(true);

    // --- 3. GRASS SIDE ---
    glGenTextures(1, &gApp.mtexture_grassblock);
    glBindTexture(GL_TEXTURE_2D, gApp.mtexture_grassblock);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("./texture/Grass_Block.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    // --- 4. GRASS TOP ---
    glGenTextures(1, &gApp.mtexture_grassblock_top);
    glBindTexture(GL_TEXTURE_2D, gApp.mtexture_grassblock_top);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("./texture/Grass_Block_Top.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
}

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

void getOpenGLStuff(){
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR){
	std::cerr << "error after setup: " << err << std::endl;
    }

}

// int main(){} function collection
void InitializeProgram(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
	std::cout << "SDL3 could not be initialized" << std::endl;
	exit(0);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //thia creates the SDL window for now
    gApp.mGraphicsApplicationWindow = SDL_CreateWindow("SDL_WINDOW", gApp.mScreenWidth, gApp.mScreenHeight, SDL_WINDOW_OPENGL);
    if(gApp.mGraphicsApplicationWindow == nullptr){
	std::cout << "SDL not working" << std::endl;
	exit(1);
    }
    gApp.mOpenGLContext = SDL_GL_CreateContext(gApp.mGraphicsApplicationWindow);
    if (gApp.mOpenGLContext == nullptr){
	std::cout << "Context not working" << std::endl;
	exit(1);
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
	std::cout << "glad not initalize" << std::endl;
	exit(1);
    }
    getOpenGLStuff();
}

//the void MainLoop() functions
void Input(){
    static int mouseX = gApp.mScreenHeight/2;
    static int mouseY = gApp.mScreenWidth/2;
    SDL_Event e;
    while (SDL_PollEvent(&e)){
    //event
	if(e.type == SDL_EVENT_QUIT){
	    std::cout << "Goodbye!!!!" << std::endl;
	    gApp.mQuit = true;
	}
	if(e.type == SDL_EVENT_MOUSE_MOTION){
	    mouseX += e.motion.xrel;
	    mouseY += e.motion.yrel;
	    gApp.mCamera.MouseLook(mouseX, mouseY);
	}
    }
    float speed = 0.1;
    const bool *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_UP]){
	gApp.mCamera.MoveForward(speed);
    }
    if(state[SDL_SCANCODE_DOWN]){
	gApp.mCamera.MoveBackward(speed);
    }
    if(state[SDL_SCANCODE_RIGHT]){
	gApp.mCamera.MoveRight(speed);
    }
    if(state[SDL_SCANCODE_LEFT]){
	gApp.mCamera.MoveLeft(speed);
    }
    if (state[SDL_SCANCODE_ESCAPE]){
	gApp.mQuit = true;
    }
}

//the camera
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
void RenderMesh(const Mesh3D* mesh, App* app) {
    //deciding your position
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(mesh->mtransform.x, mesh->mtransform.y, mesh->mtransform.z));

    //rotation
    //float time = (float)SDL_GetTicks() / 1000.0f;
    //model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    
    //tells the Vertex Shader or GPU to move to world space
    GLint u_Model = glGetUniformLocation(mesh->pipeline, "u_ModelMatrix");
    glUniformMatrix4fv(u_Model, 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app->mtexture_dirt);
    glUniform1i(glGetUniformLocation(mesh->pipeline, "Dirt"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, app->mtexture_grassblock);
    glUniform1i(glGetUniformLocation(mesh->pipeline, "GrassBlock"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, app->mtexture_grassblock_top);
    glUniform1i(glGetUniformLocation(mesh->pipeline, "GrassBlockTop"), 2);
    //recall VAO to get the texture data again
    glBindVertexArray(mesh->geometry.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->geometry.vertexCount);
}

void RenderingMultipleMesh(int x, int z, Transform mtransform){
    Mesh3D newMesh;

    //this has all the 16*16 perlin noise data so probably {1.213813,3.23891238123 .....}
    gChunkVector.meshData = gApp.mChunk.draw(x/16, z/16);
    //this have the ID number of VAO and VBO
    newMesh.geometry = MeshDataVertexSpecfication(gChunkVector);
    newMesh.mtransform = mtransform;
    newMesh.pipeline = gApp.mGraphicsPipelineShaderProgram;

    //makes a temporary pair stroing x and z
    std::pair<int, int> temp = std::make_pair(x,z);
    gSceneMeshes[temp] = newMesh;
}
void UnloadMultipleMesh(int px, int pz, int radius){
    int distacne = radius * 16;
    //loops through the whole mesh from beggining to end
    //a special way to iterate maps
    for (auto mesh = gSceneMeshes.begin(); mesh != gSceneMeshes.end();){
	//access the x of the mesh
	int chunkX = mesh->first.first;
	//access the z of the mesh
	int chunkZ = mesh->first.second;
	if (abs(chunkX - px) > distacne || abs(chunkZ - pz) > distacne){
	    glDeleteBuffers(1, &mesh->second.geometry.VBO);
	    glDeleteVertexArrays(1, &mesh->second.geometry.VAO);

	    //The Map deletes the node at the current address. 123 Main St

	    //The Map looks at its internal map and says: "The next house in line was at 125 Main St."

	    //It returns that new address to you.

	    //By writing mesh = ..., you update your address book with the new, safe address before the loop continues.
	    mesh = gSceneMeshes.erase(mesh);
	}else{
	    mesh++;
	}
    }
}
void RenderWorld(){
    int playerChunkX = glm::floor(gApp.mCamera.myEye.x / 16.0f);
    int playerChunkZ = glm::floor(gApp.mCamera.myEye.z / 16.0f);

    int radius = 7;
    for(int x = playerChunkX - radius; x <= playerChunkX + radius; x++) {
	for(int z = playerChunkZ - radius; z <= playerChunkZ + radius; z++) {
	    std::pair<int, int> chunkID = {x*16, z*16};
	    // gSceneMeshes.find() means find anything inside the bracket
	    // gSceneMeshes.end() means nothing exsist inside the gSceneMeshes
	    if (gSceneMeshes.find(chunkID) == gSceneMeshes.end()) {
		RenderingMultipleMesh(chunkID.first, chunkID.second, {float(x)*16.0f, -64.0f, float(z)*16.0f});
	    }
	}
    }
    UnloadMultipleMesh(playerChunkX * 16, playerChunkZ * 16, radius);
}
void MainLoop(){
    //setting the mouse position
    SDL_WarpMouseInWindow(gApp.mGraphicsApplicationWindow, gApp.mScreenWidth/2.0, gApp.mScreenHeight/2.0);
    SDL_SetWindowRelativeMouseMode(gApp.mGraphicsApplicationWindow, true);

    while(!gApp.mQuit){
	Input();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//setting the z buffer
	glEnable(GL_DEPTH_TEST);
	//setting the background color
	glViewport(0,0,gApp.mScreenWidth,gApp.mScreenHeight);
	glClearColor(0.f,1.f,1.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//prepeare camera
	PrepareWorld(&gApp);
	//draw the mesh
	RenderWorld();
	for (auto const& item : gSceneMeshes) {
            // Use the generic render function we built earlier
            RenderMesh(&item.second, &gApp);
        }

	SDL_GL_SwapWindow(gApp.mGraphicsApplicationWindow);
    }
}
void CleanUp(){
    Geometry3D geo;
    SDL_DestroyWindow(gApp.mGraphicsApplicationWindow);
    for (auto& item : gSceneMeshes) {
        glDeleteBuffers(1, &item.second.geometry.VBO);
        glDeleteVertexArrays(1, &item.second.geometry.VAO);
    }
    glDeleteProgram(gApp.mGraphicsPipelineShaderProgram);
    SDL_Quit();
}

int main(){
    InitializeProgram();
    LoadTexture();
    CreateGraphicsPipeline(); 
    MainLoop();
    CleanUp();
    return 0;
}
