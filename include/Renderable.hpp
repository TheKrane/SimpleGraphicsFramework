#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Material.hpp"
#include "SceneNode.hpp"

class Renderable : public SceneNode
{
public:
    Renderable(Material* pMaterial);
    Renderable(const std::string& filepath, Material* pMaterial);
    virtual ~Renderable();

    void PreRender(Renderer* pRenderer, Shader* pCurrentShader);
    void Render(Renderer* pRenderer, Shader* pCurrentShader);

    //Buffer objects
    GLuint m_VertexBuffer;
    GLuint m_UVBuffer;
    GLuint m_NormalBuffer;
    GLuint m_IndexBuffer;

    bool LoadFromObj(const std::string& filepath);

    void setMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }
    Material* getMaterial() { return m_pMaterial; }

    void SetName(const std::string& name) { m_Name = name; }
    std::string GetName() { return m_Name; }

private:
    Material* m_pMaterial;

    std::string m_Name;
};

#endif
