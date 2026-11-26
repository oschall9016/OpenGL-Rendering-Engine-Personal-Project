#pragma once

#include <SDL2/SDL.h>

class SDLWindow
{
public:
	SDLWindow(int width, int height, const char* title);
	SDLWindow();
	~SDLWindow();

	void SwapBuffers();

	void SetSize(int width, int height);
	void SetTitle(const char* title);
	void SetIcon(const char* path);

	int GetWidth() const;
	int GetHeight() const;
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
};