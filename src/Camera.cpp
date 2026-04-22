#include "Camera.h"
#include "SDLInput.h"

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
	case FORWARD:
		position += front * deltaTime;
		break;
	case BACKWARD:
		position -= front * deltaTime;
		break;
	case LEFT:
		position -= glm::normalize(glm::cross(front, up)) * deltaTime;
		break;
	case RIGHT:
		position += glm::normalize(glm::cross(front, up)) * deltaTime;
		break;
	case UP:
		position += up * deltaTime;
		break;
	case DOWN:
		position -= up * deltaTime;
		break;
	}
}

void Camera::ProcessInput(SDLInput input, float deltaTime)
{
	if (input.isKeyPressed(SDL_SCANCODE_W)) processKeyboard(FORWARD, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_S)) processKeyboard(BACKWARD, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_A)) processKeyboard(LEFT, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_D)) processKeyboard(RIGHT, deltaTime);
	
	if (input.isKeyPressed(SDL_SCANCODE_SPACE)) processKeyboard(UP, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_LCTRL)) processKeyboard(DOWN, deltaTime);
	
}
