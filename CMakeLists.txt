cmake_minimum_required(VERSION 3.10)
project(opengl2)

# 1. ADD THIS LINE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC ../../lib/glad/src/glad.c)
target_include_directories(glad 
    PUBLIC 
    ../../lib/glad/include
)
add_executable(prog 
    ./src/main.cpp 
    ./src/camera.cpp 
    ./src/stb_image.cpp
    ./src/chunck.cpp
    ./src/terrain.cpp
    ./src/infinite_generator.cpp
)

target_include_directories(prog
    PRIVATE 
    C:/WorkingFile/Clang/lib/glm-master
    C:/WorkingFile/Clang/OpenGL/OPENGL2/vendor
)
target_link_libraries(prog 
    PRIVATE 
    SDL3::SDL3 
    OpenGL::GL 
    glad 
)
