#ifndef RENDERER_HPP
#define RENDERER_HPP

#define GLM_ENABLE_EXPERIMENTAL
#define TIMESTEP 0.05

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "SceneNode.hpp"
#include "Renderable.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Texture.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(const std::string &windowTitle, unsigned int width, unsigned int heigth);

    void UpdateScene();

    void PollEvents(bool* quit);
    void Render();
    void PostRender();

    Shader* CreateShader(const std::string &filepath);

    GLFWwindow* getWindow() { return m_pWindow; }

    void AddRootNode(SceneNode* pSceneNode);
    void SetMainCamera(Camera* pCamera);
    void AddLight(Light* pLight) { m_Lights.push_back(pLight); }
    void SetClearColor(float r, float g, float b) { glClearColor(r, g, b, 0.0); }

    GLuint getCurrentShaderProgram() { return m_pCurrentShader->getShaderProgram(); }

private:
    //Window properties
    GLFWwindow* m_pWindow;
    std::string m_WindowTitle;
    unsigned int m_Width, m_Height;

    //Root renderables
    std::vector<SceneNode*> m_RootNodes;

    //Shaders
    Shader* m_pCurrentShader;
    std::vector<Shader*> m_Shaders;

    //Camera
    Camera* m_pMainCamera;

    //Lights
    std::vector<Light*> m_Lights;
};

#endif
