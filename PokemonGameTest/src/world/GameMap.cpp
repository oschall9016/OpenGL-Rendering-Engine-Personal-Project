#include "GameMap.h"

#include "Tilemap.h"

#include "Model.h"
#include "Shader.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/glm.hpp>

GameMap::GameMap(Tilemap& tileMap, std::shared_ptr<Model> quad, std::shared_ptr<Shader> shader) : tilemap(tileMap)
{
	this->quad = quad;
	this->shader = shader;
}

Tilemap& GameMap::GetTilemap()
{
	return tilemap;
}

void GameMap::DrawGameMap(Renderer& renderer, Camera& camera)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat,glm::vec3((tilemap.mapXSize - 1) / 2.0f,-1.0f,(tilemap.mapZSize - 1) / 2.0f));
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	modelMat = glm::scale(modelMat, glm::vec3(tilemap.mapXSize, tilemap.mapZSize, 0.0));
	
	shader->Bind();

	shader->setMat4("model", modelMat);
	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	shader->setFloat("mapXSize", (float)tilemap.mapXSize);
	shader->setFloat("mapZSize", (float)tilemap.mapZSize);

	shader->Unbind();

	renderer.RenderModel(*quad, *shader);
}