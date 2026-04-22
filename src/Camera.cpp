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
void Camera::ProcessInput()
{

}
