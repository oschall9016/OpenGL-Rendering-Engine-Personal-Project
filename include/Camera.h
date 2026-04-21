#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Direction
{
	Forward,
	Backward,
	Up,
	Down,
	Left,
	Right
};

class Camera
{
public:
	Camera(glm::vec3 postion, glm::vec3 front, glm::vec3 up);

	glm::mat4 createViewMatrix();
	void processKeyboard(Camera_Direction direction, float deltaTime);

	void PollEvents(bool& gameRunning);
	
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
private:
};
