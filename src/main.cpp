#include "Renderer.hpp"
#include "InputManager.hpp"
#include "Robot.hpp"
#include "OrbitCamera.hpp"
#include "Light.hpp"
#include "Material.hpp"

#include "imgui/imgui.h"

void SetStandardMaterialValues(Material* pMaterial, glm::vec4 color, Texture texture);

int main(int argc, char** argv)
{
    // create and initialize renderer
    Renderer renderer;
    if(!renderer.Initialize("Lonely (Robot) Island", 1080, 720))
        return 1;

	// initialize input manager
    InputManager inputManager(renderer.getWindow());
    
    // initialize camera
    OrbitCamera mainCamera(&inputManager);
    renderer.SetMainCamera(&mainCamera);

    //Load textures
    Texture grassTexture("data/grass.jpg");
    Texture boatTexture("data/boat.jpg");
    Texture waterTexture("data/water2.jpg");
    Texture melonTexture("data/melon.png");
    Texture sandTexture("data/sand.jpg");

    //Load shaders
    Shader* pStandardShader = renderer.CreateShader("data/StandardShader");
    Shader* pWaterShader = renderer.CreateShader("data/SimpleWaterShader");

    //Load materials
    Material islandMaterial(pStandardShader);
    islandMaterial.SetAttribute("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    islandMaterial.SetAttribute("Ambient", 1.0f);
    islandMaterial.SetAttribute("Diffuse", 1.0f);
    islandMaterial.SetAttribute("Specular", 0.5f);
    islandMaterial.SetTextureAttribute("DiffuseTexture", grassTexture);

    Material boatMaterial(pStandardShader);
    boatMaterial.SetAttribute("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    boatMaterial.SetAttribute("Ambient", 1.0f);
    boatMaterial.SetAttribute("Diffuse", 1.0f);
    boatMaterial.SetAttribute("Specular", 1.0f);
    boatMaterial.SetTextureAttribute("DiffuseTexture", boatTexture);

    Material waterMaterial(pWaterShader);
    waterMaterial.SetAttribute("Color", glm::vec4(0.5, 0.5, 1.0, 0.7));
    waterMaterial.SetAttribute("Ambient", 1.0f);
    waterMaterial.SetAttribute("Diffuse", 1.0f);
    waterMaterial.SetAttribute("Specular", 1.0f);
    waterMaterial.SetTextureAttribute("DiffuseTexture", waterTexture);

    Material melonMaterial(pStandardShader);
    melonMaterial.SetAttribute("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    melonMaterial.SetAttribute("Ambient", 1.0f);
    melonMaterial.SetAttribute("Diffuse", 1.0f);
    melonMaterial.SetAttribute("Specular", 1.0f);
    melonMaterial.SetTextureAttribute("DiffuseTexture", melonTexture);

    Material sandMaterial(pStandardShader);
    sandMaterial.SetAttribute("Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    sandMaterial.SetAttribute("Ambient", 1.0f);
    sandMaterial.SetAttribute("Diffuse", 1.0f);
    sandMaterial.SetAttribute("Specular", 1.0f);
    sandMaterial.SetTextureAttribute("DiffuseTexture", sandTexture);

    //Load and arrange objects
	Renderable island("data/island.obj", &islandMaterial);
	Renderable water("data/water.obj", &waterMaterial);
    Renderable boat("data/boat.obj", &boatMaterial);
    Renderable melon("data/melon.obj", &melonMaterial);
	Renderable sand("data/water.obj", &sandMaterial);

    Robot robot(&renderer, pStandardShader, &melon, &boat);

	renderer.AddRootNode(&island);
    renderer.AddRootNode(&water);
    renderer.AddRootNode(&boat);
    renderer.AddRootNode(&sand);
    melon.SetParent(&boat);

    //Create lights
    Light pointLight(PointLight, 0);
    pointLight.setPosition(0.0f, 1.0f, 2.0f);
    pointLight.setIntensity(1.0f, 1.0f, 1.0f);
    pointLight.setAmbient(0.02f);
    pointLight.SetParent(&boat);

    Light directionalLight(DirectionalLight, 1);
    directionalLight.setAmbient(0.3f);
    renderer.AddRootNode(&directionalLight);

    //Adjust scene
    island.setPosition(0.0, -3.1, 0.0);
    sand.setPosition(0.0, -2.5, 0.0);
    sand.setScale(1.05, 1.05, 1.05);
    water.setPosition(0.0, -2.0, 0.0);
    melon.setPosition(0.0f, 0.3f, 0.0f);

    //Runtime variables
    bool quit = false;
    float time = 0.0f;
    float timeOfDay = 12.0f;
    bool streetlightToggle = false;

    //Main loop
    while(!quit)
    {
        time += TIMESTEP;

        //Events
        renderer.PollEvents(&quit);
        renderer.UpdateScene();
        inputManager.Update();
        
        //UI
        ImGui::Begin("Settings");
        ImGui::Text("Scene Settings");
        ImGui::SliderFloat("Time of Day", &timeOfDay, 0, 24);
        ImGui::Checkbox("Boat Light", &streetlightToggle);
        robot.RobotGUI();
        ImGui::Separator();
        ImGui::Text("Controls:\nMove Camera: Hold right MB and drag\nZoom: Hold middle MB and drag");
        ImGui::End();

        //Update
        mainCamera.Update();
		robot.Update();

        if(!streetlightToggle)
            pointLight.setIntensity(0.0f, 0.0f, 0.0f);
        else
            pointLight.setIntensity(1.0f, 1.0f, 1.0f);

        //Boat
        boat.setPosition(glm::sin(time*0.2f)*7.0f, -1.8f, glm::cos(time*0.2f)*7.0f);
        boat.setRotation(0.0f, time*0.2f*180.0f/M_PI + 90.0f, 0.0f);

        //Sun
        float sunLight = glm::clamp((timeOfDay - 12.0f)/8.0f, -1.0f, 1.0f);
        float lightIntensity = 1.0f - ((sunLight) * (sunLight));
        directionalLight.setIntensity(lightIntensity, lightIntensity, lightIntensity);
        directionalLight.setRotation(180.0f, (sunLight - 0.5f) * 180.0f, 0.0f);
        renderer.SetClearColor(glm::mix(0.5f, 0.02f, glm::abs(sunLight)), glm::mix(0.8f, 0.04f, glm::abs(sunLight)), glm::mix(0.9f, 0.2f, glm::abs(sunLight)));

        //Set additional shader uniforms
        pWaterShader->BindFloat("Time", time);

		//Render
        renderer.Render();
    }

    return 0;
}