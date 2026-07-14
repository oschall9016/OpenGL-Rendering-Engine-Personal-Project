#pragma once

#include "SDLInput.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Direction
{
	FORWARD,
	BACKWARD,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	void UpdateViewMatrix();
	glm::mat4& GetViewMatrix();

	void UpdateProjectionMatrix(float fovDegrees, float screenWidth, float screenHeight);
	glm::mat4& GetProjectionMatrix();
	
	void ProcessInput(SDLInput input, float deltaTime);
	void ProcessMouse(Sint32 x, Sint32 y);
	
	
private:
	void ProcessKeyboard(Camera_Direction direction, float deltaTime);

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	glm::mat4 view;
	glm::mat4 projection;

	float yaw;
	float pitch;
};
