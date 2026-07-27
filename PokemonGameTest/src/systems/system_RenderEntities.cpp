#include "system_RenderEntities.h"

#include "component_Model.h"
#include "component_Transform.h"

system_RenderEntities::system_RenderEntities(Renderer& renderer, EntityComponentSystem& ecs, Camera& camera) : renderer(renderer), ecs(ecs), camera(camera) {}

void system_RenderEntities::RenderEntities()
{

	for (auto& entity : entities)
	{
		std::shared_ptr<Model> model = ecs.GetComponent<component_Model>(entity)->model;
		std::shared_ptr<Shader> shader = ecs.GetComponent<component_Model>(entity)->shader;

		auto position = ecs.GetComponent<component_Transform>(entity)->position;
		auto rotation = ecs.GetComponent<component_Transform>(entity)->rotation;
		auto scale = ecs.GetComponent<component_Transform>(entity)->scale;

		shader->use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		shader->setMat4("view", view);
		shader->setMat4("projection", projection);


		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, position);
		modelMat *= glm::mat4_cast(rotation);
		modelMat = glm::scale(modelMat, scale);

		shader->setMat4("model", modelMat);

		renderer.RenderModel(*model, *shader);
	}
}