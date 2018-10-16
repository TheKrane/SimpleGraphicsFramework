#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <GLFW/glfw3.h>
#include <tuple>
#include <map>
#include <string>
#include <glm/glm.hpp>

class InputManager
{
public:
    InputManager(GLFWwindow* pWindow);
    ~InputManager();

    bool BindButton(std::string buttonName, int keyCode, bool mouseButton);
    bool UnbindButton(std::string buttonName);

    bool GetButton(std::string buttonName);
    bool GetButtonDown(std::string buttonName);
    bool GetButtonUp(std::string buttonName);

    glm::vec2 GetMouseMovement() { return m_MouseMovement; }

    void Update();

private:
    GLFWwindow* m_pWindow;

    typedef std::tuple<std::string, bool, bool, bool> Button;
    std::map<int, Button> m_ButtonMap;

    glm::vec2 m_MousePosition;
    glm::vec2 m_MouseMovement;
};

#endif
