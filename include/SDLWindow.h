#pragma once

#include <SDL2/SDL.h>

class SDLWindow
{
public:
	SDLWindow(int width, int height, const char* title);
	~SDLWindow();

	void PollEvents(bool& gameRunning);
	void SwapBuffers();

	int GetWidth();
	int GetHeight();
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
	
};