#include "Camera.h"
#include "SDLInput.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <SDL2/SDL.h>

// TODO: create default vectors
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	this->position = position;
	this->front = front;
	this->up = up;
	right = glm::normalize(glm::cross(front, up));
	yaw = -90.0f;
	pitch = 0.0f;
}

glm::mat4 Camera::createViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Direction direction, float deltaTime)
{
	float speed = 2.5f;
	float cameraSpeed = speed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		position += front * cameraSpeed;
		break;
	case BACKWARD:
		position -= front * cameraSpeed;
		break;
	case LEFT:
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
		break;
	case RIGHT:
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
		break;
	case UP:
		position += up * cameraSpeed;
		break;
	case DOWN:
		position -= up * cameraSpeed;
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

void Camera::ProcessMouse(Sint32 x, Sint32 y)
{
	
	yaw += x * 0.1f;
	pitch -= y * 0.1f;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront = glm::normalize(newFront);

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	front = newFront;
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
