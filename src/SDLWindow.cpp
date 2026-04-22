#include "SDLWindow.h"

#include <SDL2/SDL.h>
#include <iostream>

// TODO: - Add Icon
SDLWindow::SDLWindow(int w, int h, const char* title) 
{
    width = w;
    height = h;

    // inititalize SDL window and context
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        std::cout << "Failed to Create SDL window" << std::endl;
        //return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    context = SDL_GL_CreateContext(window);
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

int SDLWindow::GetWidth()
{
    return width;
}
int SDLWindow::GetHeight()
{
    return height;
}