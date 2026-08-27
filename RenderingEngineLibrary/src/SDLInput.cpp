#include "SDLInput.h"

#include <SDL2/SDL.h>

#include <iostream>

SDLInput::SDLInput() 
{ 
	keysPressed = { false };
	keysPressedLastFrame = { false };

	mouseX = 0;
	mouseY = 0;
}
SDLInput::~SDLInput() {}

bool SDLInput::isKeyPressed(SDL_Scancode key)
{
	return keysPressed[key] && !keysPressedLastFrame[key];
}

bool SDLInput::isKeyHeld(SDL_Scancode key)
{
	return keysPressed[key] && keysPressedLastFrame[key];
}

void SDLInput::updateMousePosition()
{
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

void SDLInput::setKeyPressed(SDL_Scancode key, bool state)
{
	keysPressed[key] = state;
}

void SDLInput::updateLastFrameKeyStates()
{
	keysPressedLastFrame = keysPressed;
}


