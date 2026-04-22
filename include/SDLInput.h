#pragma once

#include <SDL2/SDL.h>

class SDLInput
{
public:
	SDLInput();
	~SDLInput();

	void updateKeyState();
	bool isKeyPressed(SDL_Scancode key);
	
private:
	const Uint8* keyStates;

};
