#include "Shader.h"
#include "SDLWindow.h"
#include "Camera.h"
#include "SDLInput.h"
#include "Model.h"

#include "Mesh.h"

#include "AssetManager.h"

#include "Renderer.h"

#include "c_Renderable.h"
#include "s_Render.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SparseSet.h"
#include "ComponentManager.h"
#include "EntityComponentSystem.h"

#include "Skybox.h"

#include "Framebuffer.h"

#include "DeltaTime.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char* args[])
{
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

    int maxEntities = 2;
    EntityComponentSystem ecs;

    ecs.RegisterComponent<c_Renderable>();

    auto renderSystem = std::make_shared<s_Render>(renderer, ecs, camera);
    ecs.RegisterSystem<s_Render, c_Renderable>(renderSystem);
 
    /////////////////////// Asset Loading //////////////////////////
    std::shared_ptr<Shader> basicShader = aManager.LoadShader("BackpackShader", "assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");
    
    std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/testmodel2/Test Model.obj"); //"assets/models/backpack/backpack.obj"
    //std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/backpack/backpack.obj");   

    std::shared_ptr<Texture> testSpriteTexture = aManager.LoadTexture("assets/sprites/HeroSpriteTestTrimmed.png", true);
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
    /////////////////////// //////////// ///////////////////////////

    Skybox skybox(skyboxTexture);

    Entity spriteEntity = ecs.CreateEntity(); // 0
    c_Renderable spriteRenderData = c_Renderable{testSpriteModel, testSpriteShader};
    ecs.AddComponent<c_Renderable>(spriteEntity, spriteRenderData);

    for (int i = 0; i < maxEntities; i++)
    {
        Entity entity = ecs.CreateEntity(); // 1 2 
        c_Renderable entityRenderData = c_Renderable{ backpackModel, basicShader };
        ecs.AddComponent<c_Renderable>(entity,entityRenderData);
    }
   
    Framebuffer pixelFramebuffer(renderWidth, renderHeight);

    // main game loop
    bool gameRunning = true;
    while (gameRunning)
    {

        dt.Update();
        
        pixelFramebuffer.Bind(); //

        renderer.Clear();

        renderSystem->RenderEntitites();

        renderer.RenderSkybox(skybox, *skyboxShader, camera);

        pixelFramebuffer.Unbind(windowWidth,windowHeight); //

        renderer.RenderFramebufferQuad(pixelFramebuffer, *framebufferShader);

        input.updateKeyState();

        camera.ProcessInput(input, dt.Get());
        camera.ProcessMouse(input.getMouseX(), input.getMouseY());

        window.SwapBuffers();
        
    }

    return 0;
}