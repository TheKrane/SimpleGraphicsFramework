#include "Light.hpp"

#include <iostream>

Light::Light(LightType lightType, int index)
{
    m_LightType = lightType;
    m_Intensity = glm::vec3(1.0f, 1.0f, 1.0f);
    m_Ambient = 2.0f;

    m_Index = index;
}

Light::~Light()
{

}

void Light::PreRender(Renderer* pRenderer, Shader* pCurrentShader)
{
    GLuint shaderProgram = pCurrentShader->getShaderProgram();

    //Bind Type
    GLint typeUniform = glGetUniformLocation(shaderProgram, ("Lights[" + std::to_string(m_Index) + "].Type").c_str());
    if(typeUniform == -1)
        std::cout << "Could not bind light type uniform!" << std::endl;
    glUniform1i(typeUniform, m_LightType);

    //Bind Position
    GLint positionUniform = glGetUniformLocation(shaderProgram, ("Lights[" + std::to_string(m_Index) + "].Position").c_str());
    if(positionUniform == -1)
        std::cout << "Could not bind light position uniform!" << std::endl;
    glUniform3f(positionUniform, m_GlobalPosition[0], m_GlobalPosition[1], m_GlobalPosition[2]);

    //Bind Rotation
    GLint rotationUniform = glGetUniformLocation(shaderProgram, ("Lights[" + std::to_string(m_Index) + "].Rotation").c_str());
    if(rotationUniform == -1)
        std::cout << "Could not bind light rotation uniform!" << std::endl;
    glUniform3f(rotationUniform, m_Rotation[0], m_Rotation[1], m_Rotation[2]);

    //Bind Intensity
    GLint intensityUniform = glGetUniformLocation(shaderProgram, ("Lights[" + std::to_string(m_Index) + "].Intensity").c_str());
    if(intensityUniform == -1)
        std::cout << "Could not bind light intensity uniform!" << std::endl;
    glUniform3f(intensityUniform, m_Intensity[0], m_Intensity[1], m_Intensity[2]);

    //Bind Diffuse
    GLint ambientUniform = glGetUniformLocation(shaderProgram, ("Lights[" + std::to_string(m_Index) + "].Ambient").c_str());
    if(ambientUniform == -1)
        std::cout << "Could not bind light ambient uniform!" << std::endl;
    glUniform1f(ambientUniform, m_Ambient);

    for(unsigned int i = 0; i < m_Children.size(); i++)
        m_Children[i]->PreRender(pRenderer, pCurrentShader);
}

void Light::Render(Renderer* pRenderer, Shader* pCurrentShader)
{

}
