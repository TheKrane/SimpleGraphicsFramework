#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "Renderer.hpp"
#include "Renderable.hpp"
#include "InputManager.hpp"

class Robot
{
public:
    Robot(Renderer* pRenderer, Shader* pShader, SceneNode* pMelon, SceneNode* pBoat);
    ~Robot();

    void Update();

    void RobotGUI();

    Renderable* m_pJoint[3];
    Renderable* m_pArm[3];

    Light* m_pPointLight;
private:
    InputManager* m_pInputManager;

    SceneNode* m_pMelon;
    SceneNode* m_pBoat;

    Texture* m_pArmTexture;
    Texture* m_pJointTexture;

    Material* m_pArmMaterial;
    Material* m_pJointMaterial;

    float m_AmbientFactor;
    float m_DiffuseFactor;
    float m_SpecularFactor;

    enum RobotMode {Automatic, Manual, Grabbing}; RobotMode m_RobotMode;
    bool m_LightToggle;
    float m_IdealRotations[6];
    bool m_MelonGrabbed;

    float time;
};

#endif
