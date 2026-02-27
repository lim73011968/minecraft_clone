#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#define CAMERA_HPP
#ifdef CAMERA_HPP

class Camera{
    public:
    Camera();
    glm::vec3 myEye;
    glm::mat4 GetViewMatrix() const;
    void MouseLook( float mouseX, float mouseY);
    void MoveForward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);

    float yaw;
    float pitch;

    private:
	glm::vec3 mViewDirection;
	glm::vec3 mUpVector;
	float lastX;
        float lastY;
};
#endif
