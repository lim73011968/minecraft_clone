#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/rotate_vector.hpp"
Camera::Camera(){
    myEye = glm::vec3(0.0f, 0.0f, 0.0f);
    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = 0;
    pitch = 0;
}
//dealing with camera look
glm::mat4 Camera::GetViewMatrix() const{
    //position fo the camera, the direction, the up vector(telling where the vertical line is)
    return glm::lookAt(myEye, myEye+mViewDirection, mUpVector);
}
//dealing with camera movement
void Camera::MouseLook(float MouseX, float MouseY){
    float xpos = MouseX;
    float ypos = MouseY;
    glm::vec2 currentMouse = glm::vec2(MouseX, MouseY);

    static bool firstLook=true;
    if(firstLook){
	yaw = -90.0f;
	lastX = xpos;
        lastY = ypos;
	firstLook = false;
    }
    glm::vec2 mouseDelta;

    //setting the old and new mouse position
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    
    //setting the sensitivity
    const float sensitivity = 0.1f;
    mouseDelta *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    //setting the maximunm pitch!!
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    mViewDirection = glm::normalize(direction);
}
void Camera::MoveForward(float speed){
    myEye += mViewDirection*speed;
}
void Camera::MoveBackward(float speed){
    myEye -= mViewDirection*speed;
}
void Camera::MoveLeft(float speed){
    glm::vec3 xDirection = glm::cross(mViewDirection, mUpVector);
    myEye -= xDirection*speed;
}
void Camera::MoveRight(float speed){
    glm::vec3 xDirection = glm::cross(mViewDirection, mUpVector);
    myEye += xDirection*speed;
}
