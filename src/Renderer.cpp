#include "Renderer.h"

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#include <glad/glad.h>

#include <vector>


Renderer::Renderer()
{
	
}

void Renderer::RenderModel(Model& model, Shader& shader)
{
	std::vector<Mesh>& meshes = model.GetMeshes();

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		RenderMesh(meshes[i], shader);
	}
}

void Renderer::RenderMesh(Mesh& mesh, Shader& shader)
{

	const std::vector<std::shared_ptr<Texture>>& textures = mesh.getTextures();
	const unsigned int& VAO = mesh.GetVAO();
	const std::vector<unsigned int>& indices = mesh.getIndices();

	unsigned int diffuseNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = "texture_diffuse";
		number = std::to_string(diffuseNr++);

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i]->GetID());

	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
