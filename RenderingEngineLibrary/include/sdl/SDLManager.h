#pragma once

#include "SDLWindow.h"
#include "SDLInput.h"

class SDLManager
{
public:
	SDLManager(int width, int height, const char* title);

	void ManageEvents();

	SDLWindow window;
	SDLInput input;
	
private:
};