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
#include "EntityHandler.h"
#include "SparseSet.h"
#include "ComponentManager.h"
#include "EntityComponentSystem.h"

#include <iostream>
#include <memory>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char* args[])
{

    SDLWindow window(800, 600, "Cool Window");

    Camera camera;
    SDLInput input;

    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    AssetManager aManager;
    Renderer renderer;

    int maxEntities = 2;
    EntityComponentSystem ecs;

    ecs.RegisterComponent<c_Renderable>();

    s_Render renderSystem(renderer, ecs, camera);
 
    std::shared_ptr<Shader> basicShader = aManager.LoadShader("BackpackShader", "assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");
    std::shared_ptr<Model> backpackModel = aManager.LoadModel("assets/models/backpack/backpack.obj");
   
    std::shared_ptr<Texture> testSpriteTexture = aManager.LoadTexture("assets/sprites/HeroSpriteTestTrimmed.png");
    std::shared_ptr<Shader> testSpriteShader = aManager.LoadShader("SpriteShader", "assets/shaders/spriteVertex.vs", "assets/shaders/spriteFragment.fs");
    std::shared_ptr<Model> testSpriteModel = aManager.LoadModel("SpriteModel", Model::CreateQuad(testSpriteTexture));

    Entity spriteEntity = ecs.CreateEntity();
    c_Renderable spriteRenderData = c_Renderable{testSpriteModel, testSpriteShader};
    ecs.AddComponent<c_Renderable>(spriteEntity, spriteRenderData);

   for (int i = 0; i < maxEntities; i++)
   {
       Entity entity = ecs.CreateEntity();
       c_Renderable entityRenderData = c_Renderable{ backpackModel, basicShader };
       ecs.AddComponent<c_Renderable>(entity,entityRenderData);
   }
   

    // main game loop
    bool gameRunning = true;
    while (gameRunning)
    {
        currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glClearColor(0.1f, 0.6f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderSystem.RenderEntitites();

        input.updateKeyState();
        camera.ProcessInput(input, deltaTime);
        camera.ProcessMouse(input.getMouseX(), input.getMouseY());


        window.SwapBuffers();
        
    }

    return 0;
}