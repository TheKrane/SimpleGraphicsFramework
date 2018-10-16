#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "glm/glm.hpp"
#include <map>
#include <string>
#include <GL/glew.h>

#include "Shader.hpp"
#include "Texture.hpp"

class Material
{
public:
    Material(Shader* pShader);
    ~Material();
	
    Shader* getShader() { return m_pShader; }

    void SetAttribute(const std::string& attribute, glm::vec3 value);
    void SetAttribute(const std::string& attribute, glm::vec4 value);
    void SetAttribute(const std::string& attribute, float value);
    void SetTextureAttribute(const std::string& attribute, Texture texture);
    
    void BindAttributes();

    //DEBUG TODO REMOVE
    void PrintUniforms();

private:
    Shader* m_pShader;

    std::map<std::string, glm::vec3> vec3Attributes;
    std::map<std::string, glm::vec4> vec4Attributes;
    std::map<std::string, float> floatAttributes;
    std::map<std::string, GLuint> textureAttributes;
};

#endif
