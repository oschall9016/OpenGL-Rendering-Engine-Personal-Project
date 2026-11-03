#pragma once

#include <SDL2/SDL.h>

#include <array>

class SDLInput
{
public:
	SDLInput();
	~SDLInput();

	bool isKeyPressed(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

	Sint32 getMouseX();
	Sint32 getMouseY();
	
	friend class SDLManager; // manager needs to be able to change input but user should not

private:
	void setKeyPressed(SDL_Scancode key, bool state);
	void updateMousePosition();
	void updateLastFrameKeyStates();

	std::array<bool,SDL_NUM_SCANCODES> keysPressed;
	std::array<bool, SDL_NUM_SCANCODES> keysPressedLastFrame;
	Sint32 mouseX;
	Sint32 mouseY;

};
