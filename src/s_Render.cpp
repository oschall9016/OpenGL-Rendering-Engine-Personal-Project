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


s_Render::s_Render(Renderer& renderer, ComponentManager& manager) : renderer(renderer), manager(manager)
{
}

void s_Render::RenderEntitites()
{
	for (auto& value : entities)
	{
		std::shared_ptr<Model>& model = manager.GetComponent<c_Renderable>(value)->model;
		std::shared_ptr<Shader>& shader = manager.GetComponent<c_Renderable>(value)->shader;

		int translateAmount = value * 5;

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(translateAmount, 0, 0));

		int modelLoc = glGetUniformLocation(shader->getID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

		renderer.RenderModel(*model, *shader);
	}
}