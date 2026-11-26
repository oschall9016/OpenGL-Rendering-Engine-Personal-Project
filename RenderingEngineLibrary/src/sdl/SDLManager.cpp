#include "SDLManager.h"

#include "SDLWindow.h"
#include "SDLInput.h"

#include <SDL2/SDL.h>

SDLManager::SDLManager(int width, int height, const char* title) : window(width, height, title) {}

SDLManager::SDLManager() {}

void SDLManager::ManageEvents()
{

	input.updateLastFrameKeyStates();
	input.updateMousePosition();

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			input.setKeyPressed(e.key.keysym.scancode, true);
			break;
		case SDL_KEYUP:
			input.setKeyPressed(e.key.keysym.scancode, false);
			break;
		}
	}
}

void SDLManager::SetWindowSize(int width, int height)
{
	window.SetSize(width, height);
}

void SDLManager::SetWindowTitle(const char* title)
{
	window.SetTitle(title);
}

void SDLManager::SetWindowIcon(const char* path)
{
	window.SetIcon(path);
}