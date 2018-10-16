#include "Texture.hpp"

#include <iostream>
#include "stb_image.h"

Texture::Texture(const std::string& filepath)
{
    int width, height, comp;
    unsigned char* image = stbi_load(filepath.c_str(), &width, &height, &comp, STBI_rgb_alpha);

    if(image == nullptr)
        std::cout << "Failed to load texture " << filepath << std::endl;

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
}

Texture::~Texture()
{

}

