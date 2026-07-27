#include "DeltaTime.h"
#include <SDL2/SDL.h>

DeltaTime::DeltaTime()
{
	currentFrame = 0.0f;
	deltaTime = 0.0f;
	lastFrame = 0.0f;
}

const float DeltaTime::Get() //TODO look into use of const
{
	return deltaTime;
}
void DeltaTime::Update()
{
	currentFrame = SDL_GetTicks() / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}