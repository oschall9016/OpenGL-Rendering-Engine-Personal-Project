#include "SDLWindow.h"

#include <SDL2/SDL.h>

#include <glad/glad.h>

#include <stb_image.h>

#include <iostream>

// TODO: - Add Icon
SDLWindow::SDLWindow(int w, int h, const char* title) 
{
    width = w;
    height = h;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        std::cout << "Failed to Create SDL window" << std::endl;
        //return -1; TODO: error handling
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    context = SDL_GL_CreateContext(window);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1; TODO: error handling
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

}

SDLWindow::SDLWindow()
{
    width = 800;
    height = 600;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        std::cout << "Failed to Create SDL window" << std::endl;
        //return -1; TODO: error handling
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    context = SDL_GL_CreateContext(window);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1; TODO: error handling
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);


}

SDLWindow::~SDLWindow()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLWindow::SwapBuffers()
{
    SDL_GL_SwapWindow(window);
}

void SDLWindow::SetSize(int width, int height)
{
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    glViewport(0, 0, width, height);
}

void SDLWindow::SetIcon(const char* path)
{
    // load icon image
    int width, height, nrChannels;
    unsigned char* data;
    data = stbi_load(path, &width, &height, &nrChannels, 4);
    if (!data)
    {
        std::cout << "Failed to Import Icon Image: " << path << "\n";
        stbi_image_free(data);
        return;
    }

    // create surface
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(data, width, height, 32, width * 4, SDL_PIXELFORMAT_RGBA32);
    if (!surface)
    {
        std::cout << "Failed to Create SDL_Surface from Icon Image: " << path << "\n";
        stbi_image_free(data);
        SDL_FreeSurface(surface);
        return;
    }

    SDL_SetWindowIcon(window, surface);

    stbi_image_free(data);
    SDL_FreeSurface(surface);
}

void SDLWindow::SetTitle(const char* title)
{
    SDL_SetWindowTitle(window, title);
}

int SDLWindow::GetWidth() const
{
    return width;
}
int SDLWindow::GetHeight() const
{
    return height;
}