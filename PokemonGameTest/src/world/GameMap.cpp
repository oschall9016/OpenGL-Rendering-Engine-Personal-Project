#include "GameMap.h"

#include "Tilemap.h"

#include "Model.h"
#include "Shader.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/glm.hpp>

GameMap::GameMap(Tilemap& tileMap, std::shared_ptr<Model> quad, std::shared_ptr<Shader> shader) : tileMap(tileMap)
{
	this->quad = quad;
	this->shader = shader;
}

Tilemap& GameMap::GetTilemap()
{
	return tileMap;
}

void GameMap::DrawGameMap(Renderer& renderer, Camera& camera)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(tileMap.mapCols/2, -0.5, tileMap.mapRows/2 - 0.3)); // why 0.3? do the actual math
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	modelMat = glm::scale(modelMat, glm::vec3(tileMap.mapCols, tileMap.mapRows, 0.0));
	
	shader->use();

	shader->setMat4("model", modelMat);
	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	renderer.RenderModel(*quad, *shader);

}