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

#include <iostream>
#include <memory>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char* args[])
{

    SDLWindow window(800, 600, "Cool Window");

    


   // ------------------------------------------------------------------------------------------------------------

    

    Shader basicShader("assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera camera(position, front, up);
    SDLInput input;

    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    AssetManager aManager;
    Renderer renderer;

    std::shared_ptr<Model> backpack = aManager.LoadModel("assets/models/backpack/backpack.obj");
    std::shared_ptr<Model> backpack2 = aManager.LoadModel("assets/models/backpack/backpack.obj");

    int maxEntities = 5;
    EntityHandler entityHandler(maxEntities);
    ComponentManager componentManager(maxEntities);
    componentManager.RegisterComponent<c_Renderable>();

    s_Render renderSystem(renderer, componentManager);

    for (int i = 0; i < maxEntities; i++)
    {
        Entity entity = entityHandler.CreateEntity();
        c_Renderable entityRenderData = c_Renderable{ aManager.LoadModel("assets/models/backpack/backpack.obj"), aManager.LoadShader("BackpackShader","assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs")};
        renderSystem.entities.insert(entity);
        componentManager.AddComponent(entity, entityRenderData);
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

        basicShader.use(); 

        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = camera.createViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        //int modelLoc = glGetUniformLocation(basicShader.getID(), "model");
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(basicShader.getID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(basicShader.getID(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //backpack->Draw(basicShader);
        //renderer.RenderModel(*backpack, basicShader);

        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //modelLoc = glGetUniformLocation(basicShader.getID(), "model");
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        //backpack2->Draw(basicShader);
        renderSystem.RenderEntitites();

        input.updateKeyState();
        camera.ProcessInput(input, deltaTime);
        camera.ProcessMouse(input.getMouseX(), input.getMouseY());


        window.SwapBuffers();
        
    }

    return 0;
}