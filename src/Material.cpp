#define STB_IMAGE_IMPLEMENTATION

#include "Material.hpp"

#include <iostream>
#include "stb_image.h"
#include <vector>

Material::Material(Shader* pShader)
{
    m_pShader = pShader;
}

Material::~Material()
{

}

void Material::SetAttribute(const std::string& attribute, glm::vec3 value)
{
    vec3Attributes[attribute] = value;
}

void Material::SetAttribute(const std::string& attribute, glm::vec4 value)
{
    vec4Attributes[attribute] = value;
}

void Material::SetAttribute(const std::string& attribute, float value)
{
    floatAttributes[attribute] = value;
}

void Material::SetTextureAttribute(const std::string& attribute, Texture texture)
{
    textureAttributes[attribute] = texture.GetTexture();
}

void Material::BindAttributes()
{
    for (std::map<std::string,glm::vec3>::iterator it=vec3Attributes.begin(); it!=vec3Attributes.end(); it++)
    {
        GLint uniform = glGetUniformLocation(m_pShader->getShaderProgram(), it->first.c_str());
        if(uniform == -1)
            std::cout << "Could not bind " << it->first << " uniform!" << std::endl;
        glm::vec3 vec3Attribute = it->second;
        glUniform3f(uniform, vec3Attribute[0], vec3Attribute[1], vec3Attribute[2]);
    }

    for (std::map<std::string,glm::vec4>::iterator it=vec4Attributes.begin(); it!=vec4Attributes.end(); it++)
    {
        GLint uniform = glGetUniformLocation(m_pShader->getShaderProgram(), it->first.c_str());
        if(uniform == -1)
            std::cout << "Could not bind " << it->first << " uniform!" << std::endl;
        glm::vec4 vec4Attribute = it->second;
        glUniform4f(uniform, vec4Attribute[0], vec4Attribute[1], vec4Attribute[2], vec4Attribute[3]);
    }
    
    for (std::map<std::string,float>::iterator it=floatAttributes.begin(); it!=floatAttributes.end(); it++)
    {
        GLint uniform = glGetUniformLocation(m_pShader->getShaderProgram(), it->first.c_str());
        if(uniform == -1)
            std::cout << "Could not bind " << it->first << " uniform!" << std::endl;
        float floatAttribute = it->second;
        glUniform1f(uniform, floatAttribute);
    }

    for (std::map<std::string,GLuint>::iterator it=textureAttributes.begin(); it!=textureAttributes.end(); it++)
    {
        GLint uniform = glGetUniformLocation(m_pShader->getShaderProgram(), it->first.c_str());
        if(uniform == -1)
            std::cout << "Could not bind " << it->first << " uniform to!" << std::endl;
        GLuint textureAttribute = it->second;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAttribute);
        glUniform1f(uniform, 0);
    }
}

void Material::PrintUniforms()
{
    GLint numActiveUniforms = 0;
    glGetProgramInterfaceiv(m_pShader->getShaderProgram(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);
    std::vector<GLint> values(properties.size());
    for(int attrib = 0; attrib < numActiveUniforms; ++attrib)
    {
        glGetProgramResourceiv(m_pShader->getShaderProgram(), GL_UNIFORM, attrib, properties.size(), &properties[0], values.size(), NULL, &values[0]);

        nameData.resize(values[0]); //The length of the name.
        glGetProgramResourceName(m_pShader->getShaderProgram(), GL_UNIFORM, attrib, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);

        std::cout << name << std::endl;
    }
}