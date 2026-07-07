#include "s_Render.h"
#include "Renderer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "c_Renderable.h"

#include <memory>
#include <iostream>


s_Render::s_Render(Renderer& renderer, EntityComponentSystem& ecs, Camera& camera) : renderer(renderer), ecs(ecs), camera(camera)
{
}

void s_Render::RenderEntitites()
{
	int distanceBetweenEntities = 0; // just line them up as a test

	for (auto& entity : entities)
	{
		std::shared_ptr<Model> model = ecs.GetComponent<c_Renderable>(entity)->model; // TODO: add better error checking for when wrong type is given
		std::shared_ptr<Shader> shader = ecs.GetComponent<c_Renderable>(entity)->shader;

		shader->use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		int translateAmount = distanceBetweenEntities * 5;
		distanceBetweenEntities++;

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(translateAmount, 0, 0));

		shader->setMat4("model", modelMat);

		renderer.RenderModel(*model, *shader);
	}
}