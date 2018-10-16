#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"
#include <GL/glew.h>

#include "Shader.hpp"
#include "SceneNode.hpp"

enum LightType
{
    PointLight,
    DirectionalLight
};

class Light : public SceneNode
{
public:
    Light(LightType m_LightType, int index);
    ~Light();

    void PreRender(Renderer* pRenderer, Shader* pCurrentShader);
    void Render(Renderer* pRenderer, Shader* pCurrentShader);

    void BindToShader(GLuint shaderProgram, int index);
    void setIntensity(float r, float g, float b) { m_Intensity = glm::vec3(r,g,b); }
    void setAmbient(float value) { m_Ambient = value; }

private:
    LightType m_LightType;
    glm::vec3 m_Intensity;
    float m_Ambient;

    int m_Index;
};

#endif