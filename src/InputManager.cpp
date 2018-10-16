#include "InputManager.hpp"

#include <iostream>

InputManager::InputManager(GLFWwindow* pWindow)
{
    m_pWindow = pWindow;
}

InputManager::~InputManager()
{
}

// function to add a button to the ButtonMap of the inputmanager
bool InputManager::BindButton(std::string buttonName, int keyCode, bool mouseButton)
{
    Button button(buttonName, false, false, mouseButton);
    m_ButtonMap[keyCode] = button;

    return true;  
}

// function to remove a button from the map so it cannot be callen anymore
bool InputManager::UnbindButton(std::string buttonName)
{
	// iterate over whole map
    for (auto it = m_ButtonMap.begin(); it != m_ButtonMap.end(); it++)
    {
		// if button was found by name then remove it
        if (std::get<0>(it->second) == buttonName)
        {
            m_ButtonMap.erase(it);
            return true;
        }
    }
    
    return false;
}

// function to check if button is currently pressed
bool InputManager::GetButton(std::string buttonName)
{
	// iterate over whole map
    for (auto it = m_ButtonMap.begin(); it != m_ButtonMap.end(); it++)
    {
		// if given button is found by name and it was pressed then return 'true'
        if (std::get<0>(it->second) == buttonName && std::get<1>(it->second) == true)
            return true;
    }
    
    return false;
}

// function to check if button was pressed
bool InputManager::GetButtonDown(std::string buttonName)
{
    for (auto it = m_ButtonMap.begin(); it != m_ButtonMap.end(); it++)
    {
        if (std::get<0>(it->second) == buttonName && std::get<1>(it->second) == true && std::get<2>(it->second) == false)
            return true;
    }

    return false;
}

// function to check if button was released
bool InputManager::GetButtonUp(std::string buttonName)
{
    for (auto it = m_ButtonMap.begin(); it != m_ButtonMap.end(); it++)
    {
        if (std::get<0>(it->second) == buttonName && std::get<1>(it->second) == false && std::get<2>(it->second) == true)
            return true;
    }

    return false;
}

// updating button information
void InputManager::Update()
{
    //Buttons
    for (auto it = m_ButtonMap.begin(); it != m_ButtonMap.end(); it++)
    {
        Button button = it->second;

        std::get<2>(button) = std::get<1>(button);

        if(std::get<3>(button))
        {
            if(glfwGetMouseButton(m_pWindow, it->first))
                std::get<1>(button) = true;
            else
                std::get<1>(button) = false;
        }
        else
        {
            if(glfwGetKey(m_pWindow, it->first) == GLFW_PRESS)
                std::get<1>(button) = true;
            else
                std::get<1>(button) = false;
        }

        it->second = button;
    }

    //Mouse movement
    double mouseX, mouseY;
    glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);
    m_MouseMovement = glm::vec2(mouseX, mouseY) - m_MousePosition;
    m_MousePosition = m_MousePosition + m_MouseMovement;
}
