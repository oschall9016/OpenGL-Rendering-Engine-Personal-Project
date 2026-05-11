#include "Shader.h"
#include "SDLWindow.h"
#include "Camera.h"
#include "SDLInput.h"
#include "Model.h"
#include "ModelLoader.h"

#include "NewModelLoader.h"
#include "NewMesh.h"

#include "Mesh.h"

#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char* args[])
{

    SDLWindow window(800, 600, "Cool Window");

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    

    glViewport(0, 0, window.GetWidth(), window.GetHeight());


   // ------------------------------------------------------------------------------------------------------------

    glEnable(GL_DEPTH_TEST);

    Shader basicShader("assets/shaders/BasicVertex.vs", "assets/shaders/BasicFragment.fs");
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera camera(position, front, up);
    SDLInput input;

    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    NewModelLoader loader;
    Model backpack = loader.LoadModel("assets/models/backpack/backpack.obj");

    /*

    std::vector<glm::vec3> positions =
    {
         glm::vec3(-0.5f,  0.5f, 0.0f), // top left 0
         glm::vec3(0.5f,  0.5f, 0.0f), // top right 1
         glm::vec3(0.5f, -0.5f, 0.0f), // bottom right 2
         glm::vec3(-0.5f, -0.5f, 0.0f)  // bottom left 3
    };

    float height = 32.0f;
    float width = 32.0f;

    float x = 0;
    float y = 0;

    float uMin = x / width;
    float vMin = y / height;

    float uMax = (x + width) / width;
    float vMax = (y + height) / height;

    std::vector<glm::vec2> texCoords =
    {
        {uMin, vMax}, // top left
        {uMax, vMax}, // top right
        {uMax, vMin}, // bottom right
        {uMin, vMin}  // bottom left
    };

    unsigned int id;

    id = ModelLoader::TextureFromFile(
        "OverWorld_Sprite_Hero_M_Test.png",
        "assets/sprites"
    );

    Texture texture =
    {
        "assets/sprites/OverWorld_Sprite_Hero_M_Test.png",
        "texture_diffuse",
         id
    };

    std::vector<Texture> textures;

    textures.push_back(texture);

    std::vector<unsigned int> indices =
    {
         0, 1, 3,
         1, 2, 3
    };

    std::vector<Vertex> vertices =
    {
        {positions[0], glm::vec3(0.0f), texCoords[0]},
        {positions[1], glm::vec3(0.0f), texCoords[1]},
        {positions[2], glm::vec3(0.0f), texCoords[2]},
        {positions[3], glm::vec3(0.0f), texCoords[3]}
    };
    

    Mesh Nate(vertices,indices, textures);
    */

    /////////////////////////////////

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

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = camera.createViewMatrix();

        //glm::mat4 view = glm::mat4(1.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(basicShader.getID(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(basicShader.getID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(basicShader.getID(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        backpack.Draw(basicShader);
        //Nate.Draw(basicShader);

        input.updateKeyState();
        camera.ProcessInput(input, deltaTime);
        camera.ProcessMouse(input.getMouseX(), input.getMouseY());


        window.SwapBuffers();
        
    }

    return 0;
}