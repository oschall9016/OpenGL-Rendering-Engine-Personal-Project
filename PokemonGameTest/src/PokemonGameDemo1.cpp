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

// world stuff
#include "Tile.h"
#include "Warp.h"
#include "Slope.h"
#include "Tilemap.h"
#include "GameMap.h"
#include "GameWorld.h"

#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

PokemonGameDemo1::PokemonGameDemo1()
{
    /////////////////////// Initialization /////////////////////////
    
    int windowWidth = 1440; // keep 4x3 to match render w/h
    int windowHeight = 1080;

    int renderWidth = 400; // 256 <- ds resolution feels a little too zoomed in
    int renderHeight = 300; // 192

    float playerPosX = 2.0f;
    float playerPosZ = 2.0f;
    float playerScale = 1.8f;

    SDLManager sdlManager(windowWidth, windowHeight, "Cool Window");

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
    
    camera.SetPosition(playerPosX, yOffset, playerPosZ + zOffset);
    camera.SetPitchAngle(-cameraAngle);
    
    //

    DeltaTime dt;
    //AssetManager aManager;
    Renderer renderer;
    EntityComponentSystem ecs;

    
    /////////////////////// Asset Loading //////////////////////////
    
    std::shared_ptr<Shader> basicShader = aManager.LoadShader("BackpackShader", "assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");
    std::shared_ptr<Texture> testSpriteTexture = aManager.LoadTexture("assets/sprites/HeroSpriteTestOneRow.png", true);
    std::shared_ptr<Texture> otherSpriteTexture = aManager.LoadTexture("assets/sprites/Sprite-0001.png", true);
    std::shared_ptr<Shader> testSpriteShader = aManager.LoadShader("SpriteShader", "assets/shaders/spriteVertex.vs", "assets/shaders/spriteFragment.fs");
    std::shared_ptr<Model> testSpriteModel = aManager.LoadModel("SpriteModel", Model::CreateQuad(testSpriteTexture));
    std::shared_ptr<Model> otherSpriteModel = aManager.LoadModel("OtherModel", Model::CreateQuad(otherSpriteTexture));

    std::shared_ptr<Shader> framebufferShader = aManager.LoadShader("FramebufferShader", "assets/shaders/FramebufferVertex.vs", "assets/shaders/FramebufferFragment.fs");

    std::shared_ptr<Shader> worldShader = aManager.LoadShader("WorldShader", "assets/shaders/v_tile.vs", "assets/shaders/f_tile.fs");
    std::shared_ptr<Shader> defaultShader = aManager.LoadShader("DefaultShader", "assets/shaders/v_default.vs", "assets/shaders/f_default.fs");
    /*
    std::shared_ptr<Texture> tileTexture = aManager.LoadTexture("assets/Textures/PokeTiles.png", false);

    std::shared_ptr<Model> testTreeModel = aManager.LoadModel("assets/models/PokemonTree1/PokemonTree1.obj", true);
    std::shared_ptr<Model> wallModel = aManager.LoadModel("assets/models/PokemonWall1/PokemonWall1.obj", true);
    std::shared_ptr<Model> constructHouseModel = aManager.LoadModel("assets/models/PokemonBuildingConstruction/PokemonBuildingConstruction.obj", true);
    std::shared_ptr<Model> brownContainerModel = aManager.LoadModel("assets/models/PokemonContainerBrown/PokemonContainerBrown.obj", true);
    std::shared_ptr<Model> orangeContainerModel = aManager.LoadModel("assets/models/PokemonContainerOrange/PokemonContainerOrange.obj", true);
    std::shared_ptr<Model> archModel = aManager.LoadModel("assets/models/PokemonArch/PokemonArch.obj", true);
    std::shared_ptr<Model> tankModel = aManager.LoadModel("assets/models/PokemonTank1/PokemonTank1.obj", true);
    std::shared_ptr<Model> bulldozerModel = aManager.LoadModel("assets/models/PokemonBulldozer/PokemonBulldozer.obj", true);
    std::shared_ptr<Model> beamsModel = aManager.LoadModel("assets/models/PokemonBeams/PokemonBeams.obj", true);
    std::shared_ptr<Model> lampModel = aManager.LoadModel("assets/models/PokemonLamp1/PokemonLamp1.obj", true);
    std::shared_ptr<Model> buildingAspertiaModel = aManager.LoadModel("assets/models/PokemonBuildingAspertia1/PokemonBuildingAspertia1.obj", true);
    std::shared_ptr<Model> buildingBlackCityModel = aManager.LoadModel("assets/models/PokemonBuildingBlackCity1/PokemonBuildingBlackCity1.obj", true);
    std::shared_ptr<Model> buildingMallModel = aManager.LoadModel("assets/models/PokemonBuildingMall/PokemonBuildingMall.obj", true);
    std::shared_ptr<Model> buildingRoute41 = aManager.LoadModel("assets/models/PokemonBuildingRoute41/PokemonBuildingRoute41.obj", true);
    std::shared_ptr<Model> treeBlockModel = aManager.LoadModel("assets/models/PokemonTreeBlock/PokemonTreeBlock.obj", true);
    std::shared_ptr<Model> treeBlockSmallModel = aManager.LoadModel("assets/models/PokemonTreeBlockSmall/PokemonTreeBlockSmall.obj", true);

    std::shared_ptr<Texture> signTexture = aManager.LoadTexture("assets/Textures/thanks.jpeg", true);
    std::shared_ptr<Model> signModel = aManager.LoadModel("SignModel", Model::CreateQuad(signTexture));

    std::shared_ptr<Model> twistMountainInteriorModel = aManager.LoadModel("assets/models/PokemonInteriorTwistMountain1/PokemonInteriorTwistMountain1.obj",true);
    
    /////////////////////// GameMap Setup //////////////////////////

    float tileRows = 200;
    float tileCols = 65;

    Tilemap tilemap(tileRows, tileCols);
    GameMap gameMap(tilemap, tileTexture, "assets/WorldFiles/GameWorldExample.txt");

    WarpDestination map1Destination = { &gameMap, 27.0f, 40.0f };

    glm::quat rot90Y = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotN90Y = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rot45Y = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotN45X = glm::angleAxis(glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat rot180Y = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // place walls
    float wallXLineup = 2.43f;
    float wallXSize = 6.4f;

    // left walls
    for (int i = 0; i < 3; i++)
    {
        component_Transform WallTransformData = { .position{16.05f - (wallXSize * (float)i), -1.0f, 51.0f} , .scale{0.1f} }; 
        gameMap.AddMapModel(wallModel, WallTransformData);
    }

    // middle walls
    for (int i = 0; i < 10; i++)
    {
        component_Transform WallTransformData = { .position{35.0f + wallXLineup + (wallXSize * (float)i), -1.0f, 51.0f} , .scale{0.1f} }; 
        gameMap.AddMapModel(wallModel, WallTransformData);
    }
    component_Transform WallTransformData2 = { .position{35.0f + wallXLineup * 0.545 + (wallXSize * 10.0f), -1.0f, 51.0f} , .scale{0.063f, 0.1f, 0.1f} }; 
    gameMap.AddMapModel(wallModel, WallTransformData2);

    // right walls
    for (int i = 0; i < 4; i++)
    {
        component_Transform WallTransformData = { .position{106.05f + wallXLineup + (wallXSize * (float)i), -1.0f, 51.0f} , .scale{0.1f} };
        gameMap.AddMapModel(wallModel, WallTransformData);
    }

    //// construction area
    component_Transform constructHouseTransformData = { .position{26.4f, -1.0f, 39.3f} , .scale{0.07f} };
    gameMap.AddMapModel(constructHouseModel, constructHouseTransformData);

    // back containers
    component_Transform brownContainerTransformData = { .position{22.4f, -1.0f, 34.0f}, .rotation{rot90Y}, .scale{0.11f} };
    gameMap.AddMapModel(orangeContainerModel, brownContainerTransformData);

    component_Transform brownContainerTransformData2 = { .position{28.8f, -1.0f, 34.0f}, .rotation{rot90Y}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData2);

    // right containers
    component_Transform brownContainerTransformData3 = { .position{35.0f, -1.0f, 36.0f}, .rotation{rot45Y}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData3);

    component_Transform brownContainerTransformData4 = { .position{36.0f, -1.0f, 42.2f}, .scale{0.11f} };
    gameMap.AddMapModel(orangeContainerModel, brownContainerTransformData4);

    // left containers
    component_Transform brownContainerTransformData5 = { .position{18.1f, -1.0f, 39.2f}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData5);

    component_Transform brownContainerTransformData6 = { .position{18.1f, -1.0f, 46.2f}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData6);

    // top containers
    component_Transform brownContainerTransformData7 = { .position{18.1f, 2.4f, 43.2f}, .scale{0.11f} };
    gameMap.AddMapModel(orangeContainerModel, brownContainerTransformData7);

    component_Transform brownContainerTransformData8 = { .position{28.8f, 2.4f, 34.0f}, .rotation{rot90Y}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData8);
    //

    component_Transform archTransformData = { .position{27.0f, -1.0f, 50.4f}, .rotation{rot90Y}, .scale{0.09f} };
    gameMap.AddMapModel(archModel, archTransformData);

    component_Transform tankTransformData = { .position{37.0f, -1.0f, 47.5f}, .scale{0.1f} };
    gameMap.AddMapModel(tankModel, tankTransformData);
    component_Transform tankTransformData2 = { .position{17.0f, -1.0f, 34.5f}, .scale{0.1f} };
    gameMap.AddMapModel(tankModel, tankTransformData2);

    component_Transform bulldozerTransformData = { .position{31.5f, -1.0f, 39.0f}, .rotation{rotN90Y}, .scale{0.07f} };
    gameMap.AddMapModel(bulldozerModel, bulldozerTransformData);

    component_Transform beamsTransformData = { .position{21.8f, -1.0f, 39.0f}, .rotation{rot45Y}, .scale{0.07f} };
    gameMap.AddMapModel(beamsModel, beamsTransformData);

    //// street
    component_Transform beamsTransformData2 = { .position{19.0f, -1.0f, 53.5f}, .scale{0.05f} };
    gameMap.AddMapModel(beamsModel, beamsTransformData2);

    component_Transform beamsTransformData3 = { .position{110.0f, -1.0f, 53.5f}, .scale{0.05f} };
    gameMap.AddMapModel(beamsModel, beamsTransformData3);

    // street lights
    for (int i = 0; i < 10; i++)
    {
        component_Transform lampTransformData = { .position{35.0f + (i * 10), -1.0f, 55.0f}, .scale{0.07f} };
        gameMap.AddMapModel(lampModel, lampTransformData);
    }

    // buildings
    component_Transform buildingAspertiaTransformData = { .position{43.0f, -1.0f, 44.0f}, .scale{0.15f} };
    gameMap.AddMapModel(buildingAspertiaModel, buildingAspertiaTransformData);

    component_Transform buildingBlackCityTransformData = { .position{57.0f, -1.0f, 41.5f}, .scale{0.15f} };
    gameMap.AddMapModel(buildingBlackCityModel, buildingBlackCityTransformData);

    component_Transform buildingMallTransformData = { .position{77.0f, -1.0f, 39.0f}, .scale{0.15f} };
    gameMap.AddMapModel(buildingMallModel, buildingMallTransformData);

    component_Transform WallTransformData3 = { .position{100.45f, -1.0f, 42.9f}, .scale{0.06f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData3);

    component_Transform WallTransformData4 = { .position{99.0f, -1.0f, 37.45f}, .rotation{rotN90Y}, .scale{0.14f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData4);

    component_Transform WallTransformData5 = { .position{108.8f, -1.0f, 38.2f}, .rotation{rot90Y}, .scale{0.14f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData5);

    component_Transform WallTransformData6 = { .position{103.45f, -2.3f, 42.9f}, .scale{0.03f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData6);

    component_Transform WallTransformData7 = { .position{107.25f, -1.0f, 42.9f}, .scale{0.06f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData7);

    //// trees

    // back left
    for (int i = 0; i < 5; i++)
    {
        component_Transform treeBlockTransformData = { .position{3.0f, 1.1f, 3.0f + (i * 6.0f)} , .scale{0.1f}};
        gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);
    }

    // left
    for (int i = 0; i < 3; i++)
    {
        component_Transform treeBlockSmallTransformData = { .position{3.0f, 1.1f, 33.0f + (i * 6.0f)} , .scale{0.1f}};
        gameMap.AddMapModel(treeBlockSmallModel, treeBlockSmallTransformData);
    }

    component_Transform treeBlockSmallTransformData = { .position{37.0f, 1.1f, 33.0f} , .scale{0.1f} };
    gameMap.AddMapModel(treeBlockSmallModel, treeBlockSmallTransformData);

    component_Transform treeBlockTransformData = { .position{85.0f, 1.1f, 30.0f}, .rotation{rot90Y}, .scale{0.1f} };
    gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);

    for (int i = 0; i < 2; i++)
    {
        component_Transform treeBlockTransformData = { .position{91.5f + (i * 6.0f), 1.1f, 47.0f}, .rotation{rot90Y}, .scale{0.1f} };
        gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);
    }

    for (int i = 0; i < 4; i++)
    {
        component_Transform treeBlockTransformData = { .position{114.0f + (i * 6.0f), 1.1f, 47.0f}, .rotation{rot90Y}, .scale{0.1f} };
        gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);
    }

    //// sign
    component_Transform signTransformData = { .position{104.1f, 3.0f, 20.9f}, .rotation{rotN45X}, .scale{10.0f} };
    gameMap.AddMapModel(signModel, signTransformData);

    /////////////////////////////////////////////////////////////////////////////////

    float tileRows2 = 7;
    float tileCols2 = 9;

    Tilemap tilemap2(tileRows2, tileCols2);
    GameMap gameMap2(tilemap2, tileTexture, "assets/WorldFiles/InteriorExample.txt");

    WarpDestination map2Destination = { &gameMap2, 3.0f, 8.0f, Exit_Direction::UP };

    tilemap.SetTileSignature(27, 40, Tile_Type::WARP, 0);
    tilemap2.SetTileSignature(2, 8, Tile_Type::WARP, 0);
    tilemap2.SetTileSignature(3, 8, Tile_Type::WARP, 0);
    tilemap2.SetTileSignature(4, 8, Tile_Type::WARP, 0);

    component_Transform twistTransformData = { .position{8.0f, -0.7f, 7.5f}, .scale{0.06f} };
    gameMap2.AddMapModel(twistMountainInteriorModel, twistTransformData);

    //

    Warp map1map2Warp = { {map2Destination, map1Destination} }; // spawn in map 2

    tilemap.mapWarps.push_back(&map1map2Warp);
    tilemap2.mapWarps.push_back(&map1map2Warp);

    GameWorld world;

    world.gameMaps.push_back(&gameMap);
    world.gameMaps.push_back(&gameMap2);

    world.currentMap = world.gameMaps[1];
    */

    SetupMap();
    PlaceMapModels();


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

    // register input detection
    auto playerInputReadingSystem = std::make_shared<system_player_inputDetection>(ecs, sdlManager.input);
    ecs.RegisterSystem
        <
            system_player_inputDetection,
            component_player_currentState
        >
        (playerInputReadingSystem);

    // register tile moving system
    auto playerTileMovementSystem = std::make_shared<system_player_moveTilePosition>(ecs);
    ecs.RegisterSystem
        <
            system_player_moveTilePosition,
            component_tilemapPosition,
            component_player_currentState
            
        >
        (playerTileMovementSystem);

    // register billboard/camera moving system
    auto playerRenderMovementSystem = std::make_shared<system_player_moveRenderPosition>(ecs);
    ecs.RegisterSystem
        <
            system_player_moveRenderPosition,
            component_player_currentState,
            component_tilemapPosition,
            component_Transform
        >
        (playerRenderMovementSystem);

    // register sprite animation system
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

    component_Transform playerTransformData = { .position{playerPosX, 0.0f, playerPosZ}, .scale{playerScale} };
    ecs.AddComponent<component_Transform>(playerEntity, playerTransformData);

    component_player_currentState playerStateData;
    ecs.AddComponent<component_player_currentState>(playerEntity, playerStateData);

    component_tilemapPosition playerTileData = { playerPosX, playerPosZ, &world.GetCurrentMap()->GetTilemap(),&world};
    ecs.AddComponent<component_tilemapPosition>(playerEntity, playerTileData);

    component_player_animations playerAnimationData;
    ecs.AddComponent<component_player_animations>(playerEntity, playerAnimationData);

    /////////////////////// Other Features ///////////////////////////
    
    Framebuffer pixelFramebuffer(renderWidth, renderHeight);

    /////////////////////// Main Game Loop ///////////////////////////

    bool gameRunning = true;

    while (gameRunning)
    {
        //// Game Logic

        dt.Update();

        sdlManager.ManageEvents();

        // update player info
        playerInputReadingSystem->Update(dt.Get());
        playerTileMovementSystem->Update(dt.Get());
        playerRenderMovementSystem->Update(dt.Get(), camera);
        playerAnimationSystem->Update(dt.Get());

        //// Render Frame

        pixelFramebuffer.Bind(); // do all rendering with the framebuffer binded

        renderer.Clear();

        world.GetCurrentMap()->DrawGameMap(renderer, camera);
   
        entityRenderSystem->RenderEntities();

        pixelFramebuffer.Unbind(windowWidth, windowHeight); //

        renderer.RenderFramebufferQuad(pixelFramebuffer, *framebufferShader);

        //camera.ProcessInput(sdlManager.input, dt.Get());
        //camera.ProcessMouse(sdlManager.input.getMouseX(), sdlManager.input.getMouseY());

        sdlManager.window.SwapBuffers();
    }
}

void PokemonGameDemo1::SetupMap()
{
    // load asset
    std::shared_ptr<Texture> tileTexture = aManager.LoadTexture("assets/Textures/PokeTiles.png", false);

    // create map 1
    float tileXSize = 200;
    float tileZSize = 65;

    //Tilemap tilemap(tileRows, tileCols);
    //GameMap gameMap(tilemap, tileTexture, "assets/WorldFiles/GameWorldExample.txt");
    GameMap *gameMap = world.CreateGameMap("Exterior1", tileXSize, tileZSize, tileTexture, "assets/WorldFiles/GameWorldExample.txt");

    WarpDestination map1Destination = { gameMap, 27.0f, 40.0f };

    // create map 2
    float tileXSize2 = 7;
    float tileZSize2 = 9;

    //Tilemap tilemap2(tileRows2, tileCols2);
    //GameMap gameMap2(tilemap2, tileTexture, "assets/WorldFiles/InteriorExample.txt");
    GameMap* gameMapInterior = world.CreateGameMap("Interior1", tileXSize2, tileZSize2, tileTexture, "assets/WorldFiles/InteriorExample.txt");

    // create warp
    WarpDestination map2Destination = { gameMapInterior, 3.0f, 8.0f, Exit_Direction::UP };

    //tilemap.SetTileSignature(27, 40, Tile_Type::WARP, 0);
    //tilemap2.SetTileSignature(2, 8, Tile_Type::WARP, 0);
    //tilemap2.SetTileSignature(3, 8, Tile_Type::WARP, 0);
    //tilemap2.SetTileSignature(4, 8, Tile_Type::WARP, 0);

    // warps
    Warp map1map2Warp = { {map2Destination, map1Destination} }; // spawn in map 2

    //gameMap->GetTilemap().mapWarps.push_back(map1map2Warp);
    //gameMapInterior->GetTilemap().mapWarps.push_back(map1map2Warp);

    world.AddWarp(map1map2Warp);

    gameMap->GetTilemap().SetWarp(27, 40, false, 0);
    gameMapInterior->GetTilemap().SetWarp(2, 8, false, 0);
    gameMapInterior->GetTilemap().SetWarp(3, 8, false, 0);
    gameMapInterior->GetTilemap().SetWarp(4, 8, false, 0);

    //GameWorld world;

    //world.gameMaps.push_back(&gameMap);
    //world.gameMaps.push_back(&gameMap2);

    //world.currentMap = world.gameMaps[1];
    world.SetCurrentMap("Interior1");
}

void PokemonGameDemo1::PlaceMapModels()
{
    // load assets
    std::shared_ptr<Model> testTreeModel = aManager.LoadModel("assets/models/PokemonTree1/PokemonTree1.obj", true);
    std::shared_ptr<Model> wallModel = aManager.LoadModel("assets/models/PokemonWall1/PokemonWall1.obj", true);
    std::shared_ptr<Model> constructHouseModel = aManager.LoadModel("assets/models/PokemonBuildingConstruction/PokemonBuildingConstruction.obj", true);
    std::shared_ptr<Model> brownContainerModel = aManager.LoadModel("assets/models/PokemonContainerBrown/PokemonContainerBrown.obj", true);
    std::shared_ptr<Model> orangeContainerModel = aManager.LoadModel("assets/models/PokemonContainerOrange/PokemonContainerOrange.obj", true);
    std::shared_ptr<Model> archModel = aManager.LoadModel("assets/models/PokemonArch/PokemonArch.obj", true);
    std::shared_ptr<Model> tankModel = aManager.LoadModel("assets/models/PokemonTank1/PokemonTank1.obj", true);
    std::shared_ptr<Model> bulldozerModel = aManager.LoadModel("assets/models/PokemonBulldozer/PokemonBulldozer.obj", true);
    std::shared_ptr<Model> beamsModel = aManager.LoadModel("assets/models/PokemonBeams/PokemonBeams.obj", true);
    std::shared_ptr<Model> lampModel = aManager.LoadModel("assets/models/PokemonLamp1/PokemonLamp1.obj", true);
    std::shared_ptr<Model> buildingAspertiaModel = aManager.LoadModel("assets/models/PokemonBuildingAspertia1/PokemonBuildingAspertia1.obj", true);
    std::shared_ptr<Model> buildingBlackCityModel = aManager.LoadModel("assets/models/PokemonBuildingBlackCity1/PokemonBuildingBlackCity1.obj", true);
    std::shared_ptr<Model> buildingMallModel = aManager.LoadModel("assets/models/PokemonBuildingMall/PokemonBuildingMall.obj", true);
    std::shared_ptr<Model> buildingRoute41 = aManager.LoadModel("assets/models/PokemonBuildingRoute41/PokemonBuildingRoute41.obj", true);
    std::shared_ptr<Model> treeBlockModel = aManager.LoadModel("assets/models/PokemonTreeBlock/PokemonTreeBlock.obj", true);
    std::shared_ptr<Model> treeBlockSmallModel = aManager.LoadModel("assets/models/PokemonTreeBlockSmall/PokemonTreeBlockSmall.obj", true);

    std::shared_ptr<Texture> signTexture = aManager.LoadTexture("assets/Textures/thanks.jpeg", true);
    std::shared_ptr<Model> signModel = aManager.LoadModel("SignModel", Model::CreateQuad(signTexture));

    std::shared_ptr<Model> twistMountainInteriorModel = aManager.LoadModel("assets/models/PokemonInteriorTwistMountain1/PokemonInteriorTwistMountain1.obj", true);

    // maps
    GameMap &gameMap = *world.GetMap("Exterior1");
    GameMap &gameMap2 = *world.GetMap("Interior1");

    // rotations
    glm::quat rot90Y = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotN90Y = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rot45Y = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotN45X = glm::angleAxis(glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat rot180Y = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    ////////////////////// GameMap 1 //////////////////////
    // place walls
    float wallXLineup = 2.43f;
    float wallXSize = 6.4f;

    // left walls
    for (int i = 0; i < 3; i++)
    {
        component_Transform WallTransformData = { .position{16.05f - (wallXSize * (float)i), -1.0f, 51.0f} , .scale{0.1f} };
        gameMap.AddMapModel(wallModel, WallTransformData);
    }

    // middle walls
    for (int i = 0; i < 10; i++)
    {
        component_Transform WallTransformData = { .position{35.0f + wallXLineup + (wallXSize * (float)i), -1.0f, 51.0f} , .scale{0.1f} };
        gameMap.AddMapModel(wallModel, WallTransformData);
    }
    component_Transform WallTransformData2 = { .position{35.0f + wallXLineup * 0.545 + (wallXSize * 10.0f), -1.0f, 51.0f} , .scale{0.063f, 0.1f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData2);

    // right walls
    for (int i = 0; i < 4; i++)
    {
        component_Transform WallTransformData = { .position{106.05f + wallXLineup + (wallXSize * (float)i), -1.0f, 51.0f} , .scale{0.1f} };
        gameMap.AddMapModel(wallModel, WallTransformData);
    }

    //// construction area
    component_Transform constructHouseTransformData = { .position{26.4f, -1.0f, 39.3f} , .scale{0.07f} };
    gameMap.AddMapModel(constructHouseModel, constructHouseTransformData);

    // back containers
    component_Transform brownContainerTransformData = { .position{22.4f, -1.0f, 34.0f}, .rotation{rot90Y}, .scale{0.11f} };
    gameMap.AddMapModel(orangeContainerModel, brownContainerTransformData);

    component_Transform brownContainerTransformData2 = { .position{28.8f, -1.0f, 34.0f}, .rotation{rot90Y}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData2);

    // right containers
    component_Transform brownContainerTransformData3 = { .position{35.0f, -1.0f, 36.0f}, .rotation{rot45Y}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData3);

    component_Transform brownContainerTransformData4 = { .position{36.0f, -1.0f, 42.2f}, .scale{0.11f} };
    gameMap.AddMapModel(orangeContainerModel, brownContainerTransformData4);

    // left containers
    component_Transform brownContainerTransformData5 = { .position{18.1f, -1.0f, 39.2f}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData5);

    component_Transform brownContainerTransformData6 = { .position{18.1f, -1.0f, 46.2f}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData6);

    // top containers
    component_Transform brownContainerTransformData7 = { .position{18.1f, 2.4f, 43.2f}, .scale{0.11f} };
    gameMap.AddMapModel(orangeContainerModel, brownContainerTransformData7);

    component_Transform brownContainerTransformData8 = { .position{28.8f, 2.4f, 34.0f}, .rotation{rot90Y}, .scale{0.07f} };
    gameMap.AddMapModel(brownContainerModel, brownContainerTransformData8);
    //

    component_Transform archTransformData = { .position{27.0f, -1.0f, 50.4f}, .rotation{rot90Y}, .scale{0.09f} };
    gameMap.AddMapModel(archModel, archTransformData);

    component_Transform tankTransformData = { .position{37.0f, -1.0f, 47.5f}, .scale{0.1f} };
    gameMap.AddMapModel(tankModel, tankTransformData);
    component_Transform tankTransformData2 = { .position{17.0f, -1.0f, 34.5f}, .scale{0.1f} };
    gameMap.AddMapModel(tankModel, tankTransformData2);

    component_Transform bulldozerTransformData = { .position{31.5f, -1.0f, 39.0f}, .rotation{rotN90Y}, .scale{0.07f} };
    gameMap.AddMapModel(bulldozerModel, bulldozerTransformData);

    component_Transform beamsTransformData = { .position{21.8f, -1.0f, 39.0f}, .rotation{rot45Y}, .scale{0.07f} };
    gameMap.AddMapModel(beamsModel, beamsTransformData);

    //// street
    component_Transform beamsTransformData2 = { .position{19.0f, -1.0f, 53.5f}, .scale{0.05f} };
    gameMap.AddMapModel(beamsModel, beamsTransformData2);

    component_Transform beamsTransformData3 = { .position{110.0f, -1.0f, 53.5f}, .scale{0.05f} };
    gameMap.AddMapModel(beamsModel, beamsTransformData3);

    // street lights
    for (int i = 0; i < 10; i++)
    {
        component_Transform lampTransformData = { .position{35.0f + (i * 10), -1.0f, 55.0f}, .scale{0.07f} };
        gameMap.AddMapModel(lampModel, lampTransformData);
    }

    // buildings
    component_Transform buildingAspertiaTransformData = { .position{43.0f, -1.0f, 44.0f}, .scale{0.15f} };
    gameMap.AddMapModel(buildingAspertiaModel, buildingAspertiaTransformData);

    component_Transform buildingBlackCityTransformData = { .position{57.0f, -1.0f, 41.5f}, .scale{0.15f} };
    gameMap.AddMapModel(buildingBlackCityModel, buildingBlackCityTransformData);

    component_Transform buildingMallTransformData = { .position{77.0f, -1.0f, 39.0f}, .scale{0.15f} };
    gameMap.AddMapModel(buildingMallModel, buildingMallTransformData);

    component_Transform WallTransformData3 = { .position{100.45f, -1.0f, 42.9f}, .scale{0.06f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData3);

    component_Transform WallTransformData4 = { .position{99.0f, -1.0f, 37.45f}, .rotation{rotN90Y}, .scale{0.14f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData4);

    component_Transform WallTransformData5 = { .position{108.8f, -1.0f, 38.2f}, .rotation{rot90Y}, .scale{0.14f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData5);

    component_Transform WallTransformData6 = { .position{103.45f, -2.3f, 42.9f}, .scale{0.03f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData6);

    component_Transform WallTransformData7 = { .position{107.25f, -1.0f, 42.9f}, .scale{0.06f, 0.11f, 0.1f} };
    gameMap.AddMapModel(wallModel, WallTransformData7);

    //// trees

    // back left
    for (int i = 0; i < 5; i++)
    {
        component_Transform treeBlockTransformData = { .position{3.0f, 1.1f, 3.0f + (i * 6.0f)} , .scale{0.1f} };
        gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);
    }

    // left
    for (int i = 0; i < 3; i++)
    {
        component_Transform treeBlockSmallTransformData = { .position{3.0f, 1.1f, 33.0f + (i * 6.0f)} , .scale{0.1f} };
        gameMap.AddMapModel(treeBlockSmallModel, treeBlockSmallTransformData);
    }

    component_Transform treeBlockSmallTransformData = { .position{37.0f, 1.1f, 33.0f} , .scale{0.1f} };
    gameMap.AddMapModel(treeBlockSmallModel, treeBlockSmallTransformData);

    component_Transform treeBlockTransformData = { .position{85.0f, 1.1f, 30.0f}, .rotation{rot90Y}, .scale{0.1f} };
    gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);

    for (int i = 0; i < 2; i++)
    {
        component_Transform treeBlockTransformData = { .position{91.5f + (i * 6.0f), 1.1f, 47.0f}, .rotation{rot90Y}, .scale{0.1f} };
        gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);
    }

    for (int i = 0; i < 4; i++)
    {
        component_Transform treeBlockTransformData = { .position{114.0f + (i * 6.0f), 1.1f, 47.0f}, .rotation{rot90Y}, .scale{0.1f} };
        gameMap.AddMapModel(treeBlockModel, treeBlockTransformData);
    }

    //// sign
    component_Transform signTransformData = { .position{104.1f, 3.0f, 20.9f}, .rotation{rotN45X}, .scale{10.0f} };
    gameMap.AddMapModel(signModel, signTransformData);

    ////////////////////// GameMap 2 //////////////////////
    component_Transform twistTransformData = { .position{8.0f, -0.7f, 7.5f}, .scale{0.06f} };
    gameMap2.AddMapModel(twistMountainInteriorModel, twistTransformData);
}