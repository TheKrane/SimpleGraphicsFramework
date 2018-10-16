#ifndef ORBITCAMERA_HPP
#define ORBITCAMERA_HPP

#include "Camera.hpp"
#include "InputManager.hpp"

class OrbitCamera : public Camera
{
public:
    OrbitCamera(InputManager* pInputManager);
    ~OrbitCamera();

    void Update();

private:
    InputManager* m_pInputManager;

    float m_CameraX, m_CameraY, m_CameraDistance;
};

#endif
