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


s_Render::s_Render(Renderer& renderer, EntityComponentSystem& ecs, Camera& camera) : renderer(renderer), ecs(ecs), camera(camera)
{
}

// would this confirm that each entity has all required components?
void s_Render::RenderEntitites()
{
	int distanceBetweenEntities = 0; // just line them up as a test

	auto componentSet = ecs.GetComponentSet<c_Renderable>();
	for (auto& value : componentSet->dense)
	{
		std::shared_ptr<Model>& model = value.component.model;
		std::shared_ptr<Shader>& shader = value.component.shader;

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