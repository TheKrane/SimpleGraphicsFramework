#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

#include "Texture.hpp"

class Shader
{
public:
    Shader();
    ~Shader();

    bool Initialize(const std::string &filePath);

    void Use();

    bool BindTexture(const std::string& uniformName, Texture texture);
    bool BindFloat(const std::string& uniformName, float value);

    GLuint getShaderProgram() { return m_ShaderProgram; }

private:
    bool LoadShader(const std::string &filePath, GLenum shaderType);

    GLuint m_ShaderProgram;
};

#endif