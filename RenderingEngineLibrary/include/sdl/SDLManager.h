#pragma once

#include "SDLWindow.h"
#include "SDLInput.h"

#include <string>

class SDLManager
{
public:
	SDLManager(int width, int height, const char* title);
	SDLManager();

	void ManageEvents();

	void SetWindowSize(int width, int height);
	void SetWindowTitle(const char* title);

	SDLWindow window;
	SDLInput input;
	
private:
};