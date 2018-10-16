#include "OrbitCamera.hpp"

OrbitCamera::OrbitCamera(InputManager* pInputManager)
{
    m_pInputManager = pInputManager;

    m_pInputManager->BindButton("CameraUp", GLFW_KEY_UP, false); // 'arrow-up' - move camera up
    m_pInputManager->BindButton("CameraDown", GLFW_KEY_DOWN, false); // 'arrow-down' - move camera down
    m_pInputManager->BindButton("CameraLeft", GLFW_KEY_LEFT, false); // 'arrow-left' - move camera left
    m_pInputManager->BindButton("CameraRight", GLFW_KEY_RIGHT, false); // 'arrow-right' - move camera right
    m_pInputManager->BindButton("CameraZoomin", GLFW_KEY_RIGHT_BRACKET, false); // '+' - zoom into scene
    m_pInputManager->BindButton("CameraZoomout", GLFW_KEY_SLASH, false); // '-' - zoom out of scene

    m_pInputManager->BindButton("CameraEnableRotation", GLFW_MOUSE_BUTTON_RIGHT, true);
    m_pInputManager->BindButton("CameraEnableZoom", GLFW_MOUSE_BUTTON_MIDDLE, true);

    m_CameraX = 0.0f;
    m_CameraY = 4.0f;
    m_CameraDistance = 20.0f;
}

OrbitCamera::~OrbitCamera()
{

}

void OrbitCamera::Update()
{
    // react to user input
    if(m_pInputManager->GetButton("CameraUp"))
        m_CameraY += 0.5f;
    if(m_pInputManager->GetButton("CameraDown"))
        if (m_CameraY > 0.5f)
            m_CameraY -= 0.5f;
    if(m_pInputManager->GetButton("CameraLeft"))
        m_CameraX -= 0.05f;
    if(m_pInputManager->GetButton("CameraRight"))
        m_CameraX += 0.05f;

    if(m_pInputManager->GetButton("CameraEnableRotation"))
    {
        m_CameraX -= m_pInputManager->GetMouseMovement().x * 0.01f;
        m_CameraY += m_pInputManager->GetMouseMovement().y * 0.01f;
    }
    if(m_pInputManager->GetButton("CameraEnableZoom"))
        m_CameraDistance += m_pInputManager->GetMouseMovement().y * 0.02f;

    if(m_pInputManager->GetButton("CameraZoomin"))
        m_CameraDistance -= 0.1f;
    if(m_pInputManager->GetButton("CameraZoomout"))
        m_CameraDistance += 0.1f;

    // clamp values if they are too low
    if(m_CameraY < 0.0f)
        m_CameraY = 0.0f;
    if(m_CameraDistance < 1.0f)
        m_CameraDistance = 1.0f;

    // set the camera position
    SetCameraPosition(glm::sin(m_CameraX)*m_CameraDistance, m_CameraY, glm::cos(m_CameraX)*m_CameraDistance);
}