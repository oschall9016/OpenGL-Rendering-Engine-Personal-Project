#include "PokemonGameDemo1.h"

#include "Shader.h"
#include "SDLWindow.h"
#include "Camera.h"
#include "SDLInput.h"
#include "Model.h"

#include "AssetManager.h"

#include "Renderer.h"

#include "Entity.h"
#include "EntityComponentSystem.h"

#include "Skybox.h"

#include "Framebuffer.h"

#include "DeltaTime.h"

#include "system_RenderEntities.h"
#include "system_Movement.h"

#include "component_Model.h"
#include "component_Transform.h"
#include "component_Movement.h"

#include "OverworldSpriteMovementAnimation.h"

#include <memory>
#include <vector>
#include <string>

PokemonGameDemo1::PokemonGameDemo1()
{
    /////////////////////// Initialization //////////////////////////
    int windowWidth = 1920;
    int windowHeight = 1080;

    int renderWidth = 256; //256
    int renderHeight = 144; //144

    SDLWindow window(windowWidth, windowHeight, "Cool Window");

    Camera camera;
    camera.UpdateProjectionMatrix(45.0f, (float)renderWidth, (float)renderHeight);

    SDLInput input;
    DeltaTime dt;
    AssetManager aManager;
    Renderer renderer;
    EntityComponentSystem ecs;

    
    /////////////////////// Asset Loading //////////////////////////
    //std::shared_ptr<Shader> basicShader = aManager.LoadShader("BackpackShader", "assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");

    //std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/testmodel2/Test Model.obj"); //"assets/models/backpack/backpack.obj"
    //std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/backpack/backpack.obj");   

    std::shared_ptr<Texture> testSpriteTexture = aManager.LoadTexture("assets/sprites/HeroSpriteTestOneRow.png", true);
    std::shared_ptr<Shader> testSpriteShader = aManager.LoadShader("SpriteShader", "assets/shaders/spriteVertex.vs", "assets/shaders/spriteFragment.fs");
    std::shared_ptr<Model> testSpriteModel = aManager.LoadModel("SpriteModel", Model::CreateQuad(testSpriteTexture));

    std::shared_ptr<Shader> framebufferShader = aManager.LoadShader("FramebufferShader", "assets/shaders/FramebufferVertex.vs", "assets/shaders/FramebufferFragment.fs");

    std::shared_ptr<Shader> skyboxShader = aManager.LoadShader("SkyBoxShader", "assets/shaders/SkyboxVertex.vs", "assets/shaders/SkyboxFragment.fs");
    std::vector<std::string> faces
    {
        "assets/Textures/skybox/right.jpg",
        "assets/Textures/skybox/left.jpg",
        "assets/Textures/skybox/top.jpg",
        "assets/Textures/skybox/bottom.jpg",
        "assets/Textures/skybox/front.jpg",
        "assets/Textures/skybox/back.jpg"
    };
    std::shared_ptr<Texture> skyboxTexture = aManager.LoadCubeMapTexture("/assets/Textures/skybox", faces, false);
    
    /////////////////////// ECS Setup ///////////////////////////

    ecs.RegisterComponent<component_Model>();
    ecs.RegisterComponent<component_Transform>();
    ecs.RegisterComponent<component_Movement>();

    // register render system
    auto entityRenderSystem = std::make_shared<system_RenderEntities>(renderer, ecs, camera);
    ecs.RegisterSystem
        <
            system_RenderEntities, 
            component_Model,
            component_Transform
        >
        (entityRenderSystem);

    // register movement system
    auto entityMovementSystem = std::make_shared<system_Movement>(ecs, input);
    ecs.RegisterSystem
        <
            system_Movement,
            component_Movement,
            component_Model
        >
        (entityMovementSystem);
        

    // create player entity
    Entity playerEntity = ecs.CreateEntity(); // 0

    component_Model playerModelData = component_Model{ testSpriteModel, testSpriteShader };
    ecs.AddComponent<component_Model>(playerEntity, playerModelData);

    component_Transform playerTransformData; // use defaults
    ecs.AddComponent<component_Transform>(playerEntity, playerTransformData);
    
    component_Movement playerMovementData; // use defaults
    ecs.AddComponent<component_Movement>(playerEntity, playerMovementData);

    /////////////////////// Other Features ///////////////////////////
    Framebuffer pixelFramebuffer(renderWidth, renderHeight);
    Skybox skybox(skyboxTexture);

    int animTimer = 0; 

    /////////////////////// Main Game Loop ///////////////////////////
    bool gameRunning = true;
    while (gameRunning)
    {

        dt.Update();

        pixelFramebuffer.Bind(); //

        renderer.Clear();

        animTimer++;
        if (animTimer == 25)
        {
            entityMovementSystem->Update();
            animTimer = 0;
        }
   
        entityRenderSystem->RenderEntities();

        renderer.RenderSkybox(skybox, *skyboxShader, camera);

        pixelFramebuffer.Unbind(windowWidth, windowHeight); //

        renderer.RenderFramebufferQuad(pixelFramebuffer, *framebufferShader);

        input.updateKeyState();

        //camera.ProcessInput(input, dt.Get());
        //camera.ProcessMouse(input.getMouseX(), input.getMouseY());

        window.SwapBuffers();

    }
}