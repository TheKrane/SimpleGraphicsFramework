#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera
{
public:
    Camera();
    ~Camera();

    void SetCameraPosition(float x, float y, float z);
    void ChangeCameraFOV(float deltaFOV);

    void BindCameraMatrices(GLuint shaderProgram);

    virtual void Update() = 0;

private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

    glm::vec3 m_Position = glm::vec3(0,4,-8);
    glm::vec3 m_Target = glm::vec3(0,0,0);
    glm::vec3 m_Up = glm::vec3(0,1,0);
    float m_FieldOfView = 45.0f;
};

#endif
