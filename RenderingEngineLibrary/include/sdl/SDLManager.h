#pragma once

#include "SDLWindow.h"
#include "SDLInput.h"

#include <string>

class SDLManager
{
public:
	SDLManager(int width, int height, std::string title);
	SDLManager();

	void ManageEvents();

	SDLWindow& GetWindow();
	SDLInput& GetInput();

private:
	SDLWindow window;
	SDLInput input;
};