#include "Renderer.h"

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#include "Framebuffer.h"
#include "Skybox.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>


Renderer::Renderer()
{
	
}

void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	shader.use();

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
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Renderer::RenderFramebufferQuad(Framebuffer& framebuffer,Shader& shader)
{

	glDisable(GL_DEPTH_TEST); // makes sure quad isn't effected by depth test

	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();

	// draw the quad
	glBindVertexArray(framebuffer.GetQuadVAO());
	glBindTexture(GL_TEXTURE_2D, framebuffer.GetTextureColorbufferID());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderSkybox(Skybox& skybox, Shader& shader, Camera& camera)
{
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shader.use();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shader.setMat4("view", view);
	shader.setMat4("projection", camera.GetProjectionMatrix());
	// skybox cube
	glBindVertexArray(skybox.GetCubeVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}