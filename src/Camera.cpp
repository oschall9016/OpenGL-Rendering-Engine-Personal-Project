#include "Camera.h"
#include "SDLInput.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <SDL2/SDL.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	this->position = position;
	this->front = front;
	this->up = up;
	right = glm::normalize(glm::cross(front, up));
	yaw = -90.0f;
	pitch = 0.0f;

	UpdateViewMatrix();
	UpdateProjectionMatrix(glm::radians(45.0f), 800.0f, 600.0f); // default values
}

void Camera::UpdateViewMatrix()
{
	view = glm::lookAt(position, position + front, up);
}

glm::mat4& Camera::GetViewMatrix()
{
	return view;
}

void Camera::UpdateProjectionMatrix(float fovDegrees, float screenWidth, float screenHeight)
{
	projection = glm::perspective(glm::radians(fovDegrees), screenWidth/screenHeight, 0.1f, 100.0f);
}

glm::mat4& Camera::GetProjectionMatrix()
{
	return projection;
}

void Camera::ProcessKeyboard(Camera_Direction direction, float deltaTime)
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
		position -= right * cameraSpeed;
		break;
	case RIGHT:
		position += right * cameraSpeed;
		break;
	case UP:
		position += up * cameraSpeed;
		break;
	case DOWN:
		position -= up * cameraSpeed;
		break;
	}

	UpdateViewMatrix();
}

void Camera::ProcessInput(SDLInput input, float deltaTime)
{
	if (input.isKeyPressed(SDL_SCANCODE_W)) ProcessKeyboard(FORWARD, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_S)) ProcessKeyboard(BACKWARD, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_A)) ProcessKeyboard(LEFT, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_D)) ProcessKeyboard(RIGHT, deltaTime);
	
	if (input.isKeyPressed(SDL_SCANCODE_SPACE)) ProcessKeyboard(UP, deltaTime);

	if (input.isKeyPressed(SDL_SCANCODE_LCTRL)) ProcessKeyboard(DOWN, deltaTime);
	
}

void Camera::ProcessMouse(Sint32 x, Sint32 y)
{
	
	yaw += x * 0.05f;
	pitch -= y * 0.05f;

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

	UpdateViewMatrix();
}
