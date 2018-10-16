#include "Renderer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "SceneNode.hpp"

Renderer::Renderer() : m_pCurrentShader(nullptr)
{

}

Renderer::~Renderer()
{
    // safely delete shaders
    for(unsigned int i = 0; i < m_Shaders.size(); i++)
    {
        delete m_Shaders[i];
        m_Shaders[i] = nullptr;
    }
    m_Shaders.clear();

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    // terminate glfw
    glfwTerminate();
}

bool Renderer::Initialize(const std::string &windowTitle, unsigned int width, unsigned int height)
{
    // set window properties
    m_WindowTitle = windowTitle;
    m_Width = width;
    m_Height = height;

    // initialize GLFW
    if( !glfwInit() )
    {
        std::cout <<  "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    m_pWindow = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);
    if(m_pWindow == nullptr )
    {
        std::cout << "GFLW window could not be created!" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_pWindow);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        std::cout << "Could not initialize GLEW: " << glewGetErrorString(res) << std::endl;
        return false;
    }

    // create Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // set clear color
    glClearColor(0.4, 0.8, 1.0, 0.0);
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // set OpenGL viewport
    int w, h;
    glfwGetFramebufferSize(m_pWindow, &w, &h);
    glViewport(0, 0, w, h);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(m_pWindow, true);

    return true;
}

void Renderer::UpdateScene()
{
    for(unsigned int i = 0; i < m_RootNodes.size(); i++)
        m_RootNodes[i]->Update();
}

void Renderer::PollEvents(bool* quit)
{
    glfwPollEvents();

    if(glfwWindowShouldClose(m_pWindow))
        *quit = true;

    ImGui_ImplGlfwGL3_NewFrame();
}

void Renderer::Render()
{
    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Traverse scenegraph once for each material to avoid switching shaders often
    for(unsigned int i = 0; i < m_Shaders.size(); i++)
    {
        //Bind shader
        m_pCurrentShader = m_Shaders[i];
        m_pCurrentShader->Use();

        //Bind camera matrices to shader
        m_pMainCamera->BindCameraMatrices(m_pCurrentShader->getShaderProgram());

        //Pre Render (only binds lights currently)
        for(unsigned int j = 0; j < m_RootNodes.size(); j++)
            m_RootNodes[j]->PreRender(this, m_Shaders[i]);

        //Render
        for(unsigned int j = 0; j < m_RootNodes.size(); j++)
            m_RootNodes[j]->Render(this, m_Shaders[i]);
    }

    //Render UI
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    //Swap buffers
    glfwSwapBuffers(m_pWindow);
}

Shader* Renderer::CreateShader(const std::string& filepath)
{
    Shader* pShader = new Shader();
    if(!pShader->Initialize(filepath))
        return nullptr;
        
    m_Shaders.push_back(pShader);

    return pShader;
}

// add a root-node to the current scene that is rendered
void Renderer::AddRootNode(SceneNode* pSceneNode)
{
    m_RootNodes.push_back(pSceneNode);
}

void Renderer::SetMainCamera(Camera* pCamera)
{
    m_pMainCamera = pCamera;
}
