#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <GL/glew.h>

class Texture
{
public:
    Texture(const std::string& filepath);
    ~Texture();

    GLuint GetTexture() { return m_Texture; }

private:
    GLuint m_Texture;
};

#endif
