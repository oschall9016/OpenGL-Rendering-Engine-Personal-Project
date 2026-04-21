#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>



Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	this->position = position;
	this->front = front;
	this->up = up;
}

glm::mat4 Camera::createViewMatrix()
{
	return glm::lookAt(position, position+ front, up);
}

void Camera::processKeyboard(Camera_Direction direction, float deltaTime)
{
	switch (direction)
	{
	case Forward:
		position += front * deltaTime;
		break;
	case Backward:
		position -= front * deltaTime;
		break;
	case Left:
		position -= glm::normalize(glm::cross(front, up)) * deltaTime;
		break;
	case Right:
		position += glm::normalize(glm::cross(front, up)) * deltaTime;
		break;
	}
}

// i don't like connecting the gameRunning thing here
void Camera::PollEvents(bool& gameRunning)
{
	// maybe seperate key events into own function?
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				gameRunning = false;
				break;
			case SDLK_a:
				processKeyboard(Left, 0.1f);
				break;
			case SDLK_d:
				processKeyboard(Right, 0.1f);
				break;
			case SDLK_w:
				processKeyboard(Forward, 0.1f);
				break;
			case SDLK_s:
				processKeyboard(Backward, 0.1f);
				break;
			}
		}
		else if (event.type == SDL_QUIT)
		{
			gameRunning = false;
		}
	}
}
