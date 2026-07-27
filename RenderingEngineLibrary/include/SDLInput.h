#pragma once

#include <SDL2/SDL.h>

class SDLInput
{
public:
	SDLInput();
	~SDLInput();

	void updateKeyState();
	bool isKeyPressed(SDL_Scancode key);
	Sint32 getMouseX();
	Sint32 getMouseY();
	
private:
	const Uint8* keyStates;
	Sint32 mouseX;
	Sint32 mouseY;

};
