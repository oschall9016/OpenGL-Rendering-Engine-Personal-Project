#pragma once

#include <SDL2/SDL.h>

#include <string>

class SDLWindow
{
public:
	SDLWindow(int width, int height, std::string title);
	SDLWindow();
	~SDLWindow();

	void SwapBuffers();

	void SetSize(int width, int height);
	void SetTitle(std::string title);
	void SetIcon(std::string path);

	int GetWidth();
	int GetHeight();
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
};