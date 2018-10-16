#include "Robot.hpp"
#include "Renderable.hpp"
#include "imgui/imgui.h"

float lerp(float a, float b, float f) 
{
    return (a * (1.0 - f)) + (b * f);
}

Robot::Robot(Renderer* pRenderer, Shader* pShader, SceneNode* pMelon, SceneNode* pBoat)
{
    m_pArmTexture = new Texture("data/metal2.jpg");
    m_pJointTexture = new Texture("data/metal.jpg");

    // load content for robotic arm
    m_pArmMaterial = new Material(pShader);
    m_pArmMaterial->SetAttribute("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    m_pArmMaterial->SetTextureAttribute("DiffuseTexture", *m_pArmTexture);

    m_pJointMaterial = new Material(pShader);
    m_pJointMaterial->SetAttribute("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    m_pJointMaterial->SetTextureAttribute("DiffuseTexture", *m_pJointTexture);

    m_pJoint[0] = new Renderable("data/rotJoint.obj", m_pJointMaterial);
    m_pArm[0] = new Renderable("data/robotArm.obj", m_pArmMaterial);
    m_pJoint[1] = new Renderable("data/rotJoint.obj", m_pJointMaterial);
    m_pArm[1] = new Renderable("data/robotArm.obj", m_pArmMaterial);
    m_pJoint[2] = new Renderable("data/rotJoint.obj", m_pJointMaterial);
    m_pArm[2] = new Renderable("data/robotArm.obj", m_pArmMaterial);

    //Create light
    m_pPointLight = new Light(PointLight, 2);
    m_pPointLight->setPosition(0.0f, 1.5f, 0.0f);
    m_pPointLight->setIntensity(1.0f, 1.0f, 0.0f);
    m_pPointLight->setAmbient(0.02f);

    // create scene hirarchy - robot
    pRenderer->AddRootNode(m_pJoint[0]);
    m_pArm[0]->SetParent(m_pJoint[0]);
    m_pJoint[1]->SetParent(m_pArm[0]);
    m_pArm[1]->SetParent(m_pJoint[1]);
    m_pJoint[2]->SetParent(m_pArm[1]);
    m_pArm[2]->SetParent(m_pJoint[2]); 
    m_pPointLight->SetParent(m_pArm[2]);

    m_pJoint[0]->setPosition(0.0f, 0.0f, 0.0f);
    m_pArm[0]->setPosition(0.0f, 0.5f, 0.0f);

    m_pJoint[1]->setPosition(0.0f, 2.5f, 0.0f);
    m_pArm[1]->setPosition(0.0f, 0.5f, 0.0f);

    m_pJoint[2]->setPosition(0.0f, 2.5f, 0.0f);
    m_pArm[2]->setPosition(0.0f, 0.5f, 0.0f);

    // runtime variables
    m_RobotMode = Automatic;
    m_LightToggle = false;
    m_MelonGrabbed = false;
    time = 0.0f;

    //set material factors
    m_AmbientFactor = 1.0f;
	m_SpecularFactor = 1.0f;
	m_DiffuseFactor = 1.0f;

    //set SceneNode references
    m_pMelon = pMelon;
    m_pBoat = pBoat;
}

Robot::~Robot()
{
    delete m_pJointMaterial;
    m_pJointMaterial = nullptr;

    delete m_pArmMaterial;
    m_pArmMaterial = nullptr;

    delete m_pPointLight;
    m_pPointLight = nullptr;

    delete m_pArm[2];
    m_pArm[2] = nullptr;

    delete m_pJoint[2];
    m_pJoint[2] = nullptr;

    delete m_pArm[1];
    m_pArm[1] = nullptr;

    delete m_pJoint[1];
    m_pJoint[1] = nullptr;
    
    delete m_pArm[0];
    m_pArm[0] = nullptr;

    delete m_pJoint[0];
    m_pJoint[0] = nullptr;
}

void Robot::Update()
{
    time += TIMESTEP;

    switch(m_RobotMode)
    {
        case Automatic:
            m_IdealRotations[0] = time*10.0f; //TODO: implement lerp function for angles
            m_IdealRotations[2] = glm::sin(time)*45.0f-45.0f;
            m_IdealRotations[4] = glm::cos((-1)*time)*(22.5f)-(65.5f);

            m_IdealRotations[1] = glm::sin(time * 0.5f)*45.0f;
            m_IdealRotations[3] = glm::sin(time)*(22.5f)-(65.5f);
            m_IdealRotations[5] = glm::cos((-1)*time)*45.0f-45.0f;
            break;

        case Grabbing:
            m_IdealRotations[0] = time*0.2f*180.0f/M_PI + 5.0f;
            m_IdealRotations[1] = 62.0f;
            m_IdealRotations[2] = 0.0f;
            m_IdealRotations[3] = 50.0f;
            m_IdealRotations[4] = 0.0f;
            m_IdealRotations[5] = 32.0f;
            break;
    }

    //Rotate joints
    for(int i = 0; i < 3; i++)
    {
        glm::vec3 jointRotation = m_pJoint[i]->getRotation();
        m_pJoint[i]->setRotation(jointRotation.x, lerp(m_IdealRotations[(i*2)], jointRotation.y, .9f), jointRotation.z);
    }

    //Rotate arms
    for(int i = 0; i < 3; i++)
    {
        glm::vec3 armRotation = m_pArm[i]->getRotation();
        m_pArm[i]->setRotation(lerp(m_IdealRotations[(i*2)+1], armRotation.x, .9f), armRotation.y, armRotation.z);
    }

    // Turn light on and off
    if(m_LightToggle)
        m_pPointLight->setIntensity(1.0f, 1.0f, 0.0f);
    else
        m_pPointLight->setIntensity(0.0f, 0.0f, 0.0f);

    //Set material factors
    m_pArmMaterial->SetAttribute("Ambient", m_AmbientFactor);
    m_pJointMaterial->SetAttribute("Ambient", m_AmbientFactor);

    m_pArmMaterial->SetAttribute("Diffuse", m_DiffuseFactor);
    m_pJointMaterial->SetAttribute("Diffuse", m_DiffuseFactor);

    m_pArmMaterial->SetAttribute("Specular", m_SpecularFactor);
    m_pJointMaterial->SetAttribute("Specular", m_SpecularFactor);
}

void Robot::RobotGUI()
{
    ImGui::Separator();
    ImGui::Text("Robot Material Settings");
    ImGui::SliderFloat("Ambient Factor", &m_AmbientFactor, 0, 1);
    ImGui::SliderFloat("Diffuse Factor", &m_DiffuseFactor, 0, 1);
    ImGui::SliderFloat("Specular Factor", &m_SpecularFactor, 0, 1);

    ImGui::Separator();
    ImGui::Text("Robot Controls");
    ImGui::Checkbox("Light", &m_LightToggle);
    const char* const listItems[] = {"Automatic","Manual","Grabbing"};
    int mode = static_cast<int>(m_RobotMode);
    ImGui::ListBox("Robot Mode", &mode, listItems, 3);
    m_RobotMode = static_cast<RobotMode>(mode);
    if(m_RobotMode == Manual)
    {
        ImGui::SliderFloat("Joint One", &m_IdealRotations[0], -180, 180);
        ImGui::SliderFloat("Arm One", &m_IdealRotations[1], -90, 90);
        ImGui::SliderFloat("Joint Two", &m_IdealRotations[2], -180, 180);
        ImGui::SliderFloat("Arm Two", &m_IdealRotations[3], -90, 90);
        ImGui::SliderFloat("Joint Three", &m_IdealRotations[4], -180, 180);
        ImGui::SliderFloat("Arm Three", &m_IdealRotations[5], -90, 90);
    }
    if(m_RobotMode == Grabbing)
    {
        if(ImGui::Button(m_MelonGrabbed ? "Place Melon" : "Grab Melon"))
        {
            m_MelonGrabbed = !m_MelonGrabbed;
            if(m_MelonGrabbed) 
            {
                m_pMelon->SetParent(m_pArm[2]);
                m_pMelon->setPosition(0.0f, 2.8f, 0.0f);
            } 
            else
            {
                m_pMelon->SetParent(m_pBoat);
                m_pMelon->setPosition(0.0f, 0.3f, 0.0f);
            }
        }
    }
}