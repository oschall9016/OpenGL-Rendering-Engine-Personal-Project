#include "SDLInput.h"

#include <SDL2/SDL.h>

SDLInput::SDLInput() { keyStates = SDL_GetKeyboardState(nullptr); }
SDLInput::~SDLInput() {}

bool SDLInput::isKeyPressed(SDL_Scancode key)
{
	return keyStates[key];
}

void SDLInput::updateKeyState()
{
	SDL_PumpEvents();
	keyStates = SDL_GetKeyboardState(nullptr);
}


