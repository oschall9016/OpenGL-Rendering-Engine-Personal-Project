#include "PokemonGameDemo1.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "SDLManager.h"

#include "AssetManager.h"

#include "Renderer.h"

#include "Entity.h"
#include "EntityComponentSystem.h"

#include "Skybox.h"

#include "Framebuffer.h"

#include "DeltaTime.h"

#include "system_RenderEntities.h"

// testing new systems for movement
#include "system_player_inputDetection.h"
#include "system_player_moveTilePosition.h"
#include "system_player_moveRenderPosition.h"
#include "system_player_playAnimation.h"

#include "component_Model.h"
#include "component_Transform.h"

// testing new components for movement
#include "component_player_currentState.h"
#include "component_tilemapPosition.h"
#include "component_player_animations.h"


#include "Tilemap.h"
#include "GameMap.h"

#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <iostream>

#include <glm/glm.hpp>

PokemonGameDemo1::PokemonGameDemo1()
{
    /////////////////////// Initialization /////////////////////////
    
    int windowWidth = 1920;
    int windowHeight = 1080;

    int renderWidth = 640; // 480
    int renderHeight = 360; // 270

    SDLManager sdlManager(windowWidth, windowHeight, "Cool Window");

    //SDLWindow window(windowWidth, windowHeight, "Cool Window");

    Camera camera;
    camera.UpdateProjectionMatrix(45.0f, (float)renderWidth, (float)renderHeight);

    // center the player sprite

    float cameraAngle = 45.0f; // amount camera is angled down at player
    float cameraFOV = 45.0f; // TODO: make FOV a member of camera for easier changing e.g. change the UpdateProjectionMatrix function

    float billboardSize = 1.8f; // in world coords
    float spriteSize = 32.0f; // in pixels

    // finds the distance at which the sprite is exactly 32x32 pixels in the window
    float screenScale = billboardSize * ((float)renderHeight / spriteSize);
    float cameraDistance = (screenScale / 2.0f) / std::tan(glm::radians(cameraFOV / 2.0f));

    // centers the camera on the player sprite
    float yOffset = std::sin(glm::radians(90.0f - cameraAngle)) * cameraDistance;
    float zOffset = std::cos(glm::radians(90.0f - cameraAngle)) * cameraDistance;
    
    camera.SetPosition(0.0f, yOffset, zOffset);
    camera.SetPitchAngle(-cameraAngle);

    //

    DeltaTime dt;
    AssetManager aManager;
    Renderer renderer;
    EntityComponentSystem ecs;

    
    /////////////////////// Asset Loading //////////////////////////
    
    //std::shared_ptr<Shader> basicShader = aManager.LoadShader("BackpackShader", "assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");

    //std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/testmodel2/Test Model.obj"); //"assets/models/backpack/backpack.obj"
    //std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/backpack/backpack.obj");   

    std::shared_ptr<Texture> testSpriteTexture = aManager.LoadTexture("assets/sprites/HeroSpriteTestOneRow.png", true);
    std::shared_ptr<Texture> otherSpriteTexture = aManager.LoadTexture("assets/sprites/Sprite-0001.png", true);
    std::shared_ptr<Shader> testSpriteShader = aManager.LoadShader("SpriteShader", "assets/shaders/spriteVertex.vs", "assets/shaders/spriteFragment.fs");
    std::shared_ptr<Model> testSpriteModel = aManager.LoadModel("SpriteModel", Model::CreateQuad(testSpriteTexture));
    std::shared_ptr<Model> otherSpriteModel = aManager.LoadModel("OtherModel", Model::CreateQuad(otherSpriteTexture));

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

    std::shared_ptr<Shader> worldShader = aManager.LoadShader("WorldShader", "assets/shaders/BasicVertex.vs", "assets/shaders/BasicFragment.fs");
    
    /////////////////////// GameMap Setup //////////////////////////

    int tileRows = 10;
    int tileCols = 10;

    Tilemap tilemap(tileRows, tileCols);
    GameMap gameMap(tilemap, Model::CreateEmptyQuad(), worldShader);

    /////////////////////// ECS Setup //////////////////////////////

    ecs.RegisterComponent<component_Model>();
    ecs.RegisterComponent<component_Transform>();

    ecs.RegisterComponent<component_player_currentState>();
    ecs.RegisterComponent<component_tilemapPosition>();
    ecs.RegisterComponent<component_player_animations>();



    // register render system
    auto entityRenderSystem = std::make_shared<system_RenderEntities>(renderer, ecs, camera);
    ecs.RegisterSystem
        <
            system_RenderEntities, 
            component_Model,
            component_Transform
        >
        (entityRenderSystem);

    // register new input detection
    auto playerInputReadingSystem = std::make_shared<system_player_inputDetection>(ecs, sdlManager.input);
    ecs.RegisterSystem
        <
            system_player_inputDetection,
            component_player_currentState
        >
        (playerInputReadingSystem);

    // register new tile moving system
    auto playerTileMovementSystem = std::make_shared<system_player_moveTilePosition>(ecs);
    ecs.RegisterSystem
        <
            system_player_moveTilePosition,
            component_tilemapPosition,
            component_player_currentState
            
        >
        (playerTileMovementSystem);

    // register new billboard/camera moving system
    auto playerRenderMovementSystem = std::make_shared<system_player_moveRenderPosition>(ecs);
    ecs.RegisterSystem
        <
            system_player_moveRenderPosition,
            component_player_currentState,
            component_tilemapPosition,
            component_Transform
        >
        (playerRenderMovementSystem);

    auto playerAnimationSystem = std::make_shared<system_player_playAnimation>(ecs);
    ecs.RegisterSystem
        <
            system_player_playAnimation,
            component_player_currentState,
            component_Model,
            component_player_animations
        >
        (playerAnimationSystem);


    // create player entity
    Entity playerEntity = ecs.CreateEntity(); // 0

    component_Model playerModelData = component_Model{ testSpriteModel, testSpriteShader };
    ecs.AddComponent<component_Model>(playerEntity, playerModelData);

    component_Transform playerTransformData = { .scale{1.8f} }; // use defaults
    ecs.AddComponent<component_Transform>(playerEntity, playerTransformData);

    component_player_currentState playerStateData; // use defaults
    ecs.AddComponent<component_player_currentState>(playerEntity, playerStateData);

    component_tilemapPosition playerTileData = {0.0f, 0.0f, &gameMap.GetTilemap()};
    ecs.AddComponent<component_tilemapPosition>(playerEntity, playerTileData);

    component_player_animations playerAnimationData; // use defaults
    ecs.AddComponent<component_player_animations>(playerEntity, playerAnimationData);

    ////// test
    Entity testEntity = ecs.CreateEntity(); // 1
    component_Model testModelData = component_Model{ otherSpriteModel, testSpriteShader };
    ecs.AddComponent<component_Model>(testEntity, testModelData);
    component_Transform testTransformData = { .scale{1.8f} }; // use defaults
    ecs.AddComponent<component_Transform>(testEntity, testTransformData);


    /////////////////////// Other Features ///////////////////////////
    
    Framebuffer pixelFramebuffer(renderWidth, renderHeight);
    Skybox skybox(skyboxTexture);

    /////////////////////// Main Game Loop ///////////////////////////

    bool gameRunning = true;

    while (gameRunning)
    {
        //// Game Logic

        dt.Update();

        sdlManager.ManageEvents();

        // update player
        playerInputReadingSystem->Update(dt.Get());
        playerTileMovementSystem->Update(dt.Get());
        playerRenderMovementSystem->Update(dt.Get(), camera);
        playerAnimationSystem->Update(dt.Get());

        //// Render Frame

        pixelFramebuffer.Bind(); //

        renderer.Clear();

        gameMap.DrawGameMap(renderer,camera);
   
        entityRenderSystem->RenderEntities();

        renderer.RenderSkybox(skybox, *skyboxShader, camera);

        pixelFramebuffer.Unbind(windowWidth, windowHeight); //

        renderer.RenderFramebufferQuad(pixelFramebuffer, *framebufferShader);

        //camera.ProcessInput(input, dt.Get());
        //camera.ProcessMouse(input.getMouseX(), input.getMouseY());

        sdlManager.window.SwapBuffers();
    }
}