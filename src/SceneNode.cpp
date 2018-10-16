#include "SceneNode.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <algorithm>
#include <iostream>
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

SceneNode::SceneNode() : m_ModelMatrix(1.0f), m_Position(0,0,0), m_Scale(1,1,1), m_Rotation(0,0,0), m_pParent(nullptr)
{

}

SceneNode::~SceneNode()
{

}

void SceneNode::Update(glm::mat4 parentModelMatrix)
{
    m_ModelMatrix = parentModelMatrix * glm::translate(m_Position) *  glm::mat4_cast(glm::quat(glm::radians(m_Rotation))) * glm::scale(m_Scale);

    //Get values from global model matrix
    m_GlobalPosition = glm::vec3(m_ModelMatrix[3]);

    for(unsigned int i = 0; i < m_Children.size(); i++)
        m_Children[i]->Update(m_ModelMatrix);
}

void SceneNode::SetParent(SceneNode* pParent)
{
    if(m_pParent != nullptr)
        m_pParent->RemoveChild(this);

    m_pParent = pParent;

    if(m_pParent != nullptr)
        m_pParent->AddChild(this);
}

void SceneNode::AddChild(SceneNode* pSceneNode)
{
    m_Children.push_back(pSceneNode);
}

void SceneNode::RemoveChild(SceneNode* pSceneNode)
{
    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), pSceneNode), m_Children.end()); //SEGFAULT
}