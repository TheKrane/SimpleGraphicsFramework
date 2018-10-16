#include "Camera.hpp"

#include <iostream>

Camera::Camera()
{
    int m_Width = 1080, m_Height = 720;

    m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 1000.0f);
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
}

Camera::~Camera()
{

}

void Camera::BindCameraMatrices(GLuint shaderProgram)
{
    GLint projectionUniform = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
    if (projectionUniform == -1) 
        std::cout << "Could not bind projection matrix uniform!" << std::endl;
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &m_ProjectionMatrix[0][0]);
    
    GLint ViewUniform = glGetUniformLocation(shaderProgram, "ViewMatrix");
    if (ViewUniform == -1) 
        std::cout << "Could not bind view matrix uniform" << std::endl;
    glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, &m_ViewMatrix[0][0]);

    GLint PositionUniform = glGetUniformLocation(shaderProgram, "CameraPosition");
    if (ViewUniform == -1) 
        std::cout << "Could not bind camera position uniform" << std::endl;
    glUniform3f(PositionUniform, m_Position[0], m_Position[1], m_Position[2]);
}


// set camera position
void Camera::SetCameraPosition(float x, float y, float z)
{
    // set camra position vector
    m_Position = glm::vec3(x, y, z);

    // update view matrix
	m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
}

// change camera field of view (zoom in and out)
void Camera::ChangeCameraFOV(float deltaFOV)
{
    // change FOV value
    m_FieldOfView += deltaFOV;

    // check if zoom is still in valid range
    if(m_FieldOfView < 0.0f)
        m_FieldOfView = 0.0f;
    else if(m_FieldOfView > 180.0f)
        m_FieldOfView = 180.0f;

    // update projection matrix
    int m_Width = 1080, m_Height = 720;

    m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 1000.0f);
}