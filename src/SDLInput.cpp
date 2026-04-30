#include "SDLInput.h"

#include <SDL2/SDL.h>

SDLInput::SDLInput() 
{ 
	keyStates = SDL_GetKeyboardState(nullptr);
	mouseX = 0;
	mouseY = 0;
}
SDLInput::~SDLInput() {}

bool SDLInput::isKeyPressed(SDL_Scancode key)
{
	return keyStates[key];
}

void SDLInput::updateKeyState()
{
	SDL_PumpEvents();
	keyStates = SDL_GetKeyboardState(nullptr);
	SDL_GetRelativeMouseState(&mouseX, &mouseY);
}

Sint32 SDLInput::getMouseX()
{
	return mouseX;
}
Sint32 SDLInput::getMouseY()
{
	return mouseY;
}


