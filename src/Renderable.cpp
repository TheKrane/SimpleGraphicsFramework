#include "Renderable.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.hpp"
#include "OBJ_Loader.h"

std::vector<std::string> split(const std::string& str, const char& ch);

Renderable::Renderable(Material* pMaterial) : SceneNode()
{
    m_pMaterial = pMaterial;
    m_Name = "New Renderable";
}

Renderable::Renderable(const std::string& filepath, Material* pMaterial) : SceneNode()
{
    LoadFromObj(filepath);
    m_pMaterial = pMaterial;
    m_Name = "New Renderable";
}

Renderable::~Renderable()
{

}

void Renderable::PreRender(Renderer* pRenderer, Shader* pCurrentShader)
{
    for(unsigned int i = 0; i < m_Children.size(); i++)
        m_Children[i]->PreRender(pRenderer, pCurrentShader);
}

void Renderable::Render(Renderer* pRenderer, Shader* pCurrentShader)
{
    //Only render the model if rendering the current shader pass
    if(pCurrentShader == m_pMaterial->getShader())
    {
        // bind buffers
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); 

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        GLint size; 
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

        // bind model matrix to shader
        GLint modelMatrixUniform = glGetUniformLocation(pRenderer->getCurrentShaderProgram(), "ModelMatrix");
        if (modelMatrixUniform == -1) 
            std::cout << "Could not bind model matrix uniform!" << std::endl;
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, &(m_ModelMatrix[0][0]));

        // bind other attributes
        m_pMaterial->BindAttributes();

        // draw the renderable
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

        // disable vertex attributes
        glDisableVertexAttribArray(0);   
    }

    for(unsigned int i = 0; i < m_Children.size(); i++)
        m_Children[i]->Render(pRenderer, pCurrentShader);
}

bool Renderable::LoadFromObj(const std::string& filepath)
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> uvs;
    std::vector<GLfloat> normals;
    std::vector<GLushort> indices;

    objl::Loader objLoader;
    if(!objLoader.LoadFile(filepath))
    {
        std::cout << "Could not load model: " << filepath << std::endl;
        return false;
    }

    objl::Mesh loadedMesh = objLoader.LoadedMeshes[0];

    for(unsigned int i = 0; i < loadedMesh.Vertices.size(); i++)
    {
        // load vertices
        vertices.push_back(loadedMesh.Vertices[i].Position.X);
        vertices.push_back(loadedMesh.Vertices[i].Position.Y);
        vertices.push_back(loadedMesh.Vertices[i].Position.Z);
    
        // load uvs
        uvs.push_back(loadedMesh.Vertices[i].TextureCoordinate.X);
        uvs.push_back(loadedMesh.Vertices[i].TextureCoordinate.Y);

        // load normals
        normals.push_back(loadedMesh.Vertices[i].Normal.X);
        normals.push_back(loadedMesh.Vertices[i].Normal.Y);
        normals.push_back(loadedMesh.Vertices[i].Normal.Z);
    }

    for(unsigned int i = 0; i < loadedMesh.Indices.size(); i++)
    {
        // load indices
        indices.push_back(loadedMesh.Indices[i]);
    }

    //Generate buffers
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_NormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

    return true;
}


// source: https://stackoverflow.com/questions/890164/how-can-i-split-a-string-by-a-delimiter-into-an-array
std::vector<std::string> split(const std::string& str, const char& ch)
{
    std::string next;
    std::vector<std::string> result;

    // For each character in the string
    for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
}