#include "GameMap.h"

#include "Tilemap.h"

#include "Model.h"
#include "Shader.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/glm.hpp>

#include <cmath>


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

	for (float i = 0; i < tilemap.mapXSize; i++)
	{
		for (float j = 0; j < tilemap.mapZSize; j++)
		{
			auto& tile = tilemap.GetTile(i, j);
			glm::vec3 pos = glm::vec3(i, tilemap.GetTileHeight(i, j) - 1.0f, j); 

			if (tile.signature & Tile_Type::SLOPE)
			{
				auto& slope = tilemap.slopes.at((j * tilemap.mapXSize) + i);
				glm::vec3 slopeTilePos = glm::vec3(pos.x, pos.y - 0.5f, pos.z); // slopes tiles are at height + 1 logically, but should appear at height visually
				glm::vec3 slopePos = glm::vec3(pos.x, pos.y, pos.z);

				DrawTile(slopeTilePos, camera, renderer);
				DrawSlope(slopePos, slope, camera, renderer);
			}
			else
			{
				DrawTile(pos, camera, renderer);
			}



		}
	}

}

void GameMap::DrawTile(glm::vec3 pos, Camera& camera, Renderer& renderer)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

	shader->Bind();

	shader->setMat4("model", modelMat);
	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	shader->setFloat("x", pos.x);
	shader->setFloat("z", pos.z);

	shader->Unbind();

	renderer.RenderModel(*quad, *shader);
}

void GameMap::DrawSlope(glm::vec3 pos, Slope slope, Camera& camera, Renderer& renderer)
{
	float direction = static_cast<float>(slope.direction);

	float scaleFactor = std::sqrtf(2.0f);
	float slopeAngle = std::atan(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::rotate(modelMat, glm::radians(direction), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, slopeAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::scale(modelMat, glm::vec3(1.0f, scaleFactor, 1.0f));

	shader->Bind();

	shader->setMat4("model", modelMat);
	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	shader->setFloat("x", pos.x);
	shader->setFloat("z", pos.z);

	shader->Unbind();

	renderer.RenderModel(*quad, *shader);
}