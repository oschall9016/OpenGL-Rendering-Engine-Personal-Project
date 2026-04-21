#pragma once

#include <SDL2/SDL.h>

class SDLInput
{
public:
	SDLInput();
	~SDLInput();

	void setKey(SDL_Scancode,bool pressed);
	bool isKeyUp(SDL_Scancode key);
	bool isKeyDown(SDL_Scancode key);
private:
	bool keyboardKeys[SDL_NUM_SCANCODES];

};
