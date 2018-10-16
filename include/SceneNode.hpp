#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "glm/glm.hpp"
#include <vector>

#include "Shader.hpp"

class Renderer;

class SceneNode
{
public:
    SceneNode();
    virtual ~SceneNode();

    void Update(glm::mat4 parentModelMatrix = glm::mat4(1.0f));
    virtual void PreRender(Renderer* pRenderer, Shader* pCurrentShader) = 0;
    virtual void Render(Renderer* pRenderer, Shader* pCurrentShader) = 0;

    //Getters and setters
    void setPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
    void setScale(float x, float y, float z) { m_Scale = glm::vec3(x, y, z); }
    void setRotation(float x, float y, float z) { m_Rotation = glm::vec3(x, y, z); }
    void setRotation(glm::vec3 rotation) { m_Rotation = rotation; }

    glm::vec3 getRotation() { return m_Rotation; }

    glm::vec3 getGlobalPosition() { return m_GlobalPosition; }

    void SetParent(SceneNode* pParent);
    SceneNode* GetParent() { return m_pParent; }

    //Model matrix
    glm::mat4 m_ModelMatrix;

protected:
    void AddChild(SceneNode* pSceneNode);
    void RemoveChild(SceneNode* pSceneNode);

    glm::mat4 GetGlobalModelMatrix();

    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

    glm::vec3 m_GlobalPosition;

    SceneNode* m_pParent;

    std::vector<SceneNode*> m_Children;
};

#endif
