#include "s_Render.h"
#include "Renderer.h"
#include "Shader.h"
#include "Model.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "c_Renderable.h"

#include <memory>


s_Render::s_Render(Renderer& renderer, EntityComponentSystem& ecs) : renderer(renderer), ecs(ecs)
{
}

void s_Render::RenderEntitites()
{
	int distanceBetweenEntities = 0; // just line them up as a test

	auto componentSet = ecs.GetComponentSet<c_Renderable>();
	for (auto& value : componentSet->dense)
	{
		std::shared_ptr<Model>& model = value.component.model;
		std::shared_ptr<Shader>& shader = value.component.shader;

		int translateAmount = distanceBetweenEntities * 5;
		distanceBetweenEntities++;

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(translateAmount, 0, 0));

		int modelLoc = glGetUniformLocation(shader->getID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

		renderer.RenderModel(*model, *shader);
	}
}