#include "GameMap.h"

#include "Tilemap.h"

#include "Model.h"
#include "Mesh.h"
#include "Shader.h"

#include "Renderer.h"
#include "Camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cmath>
#include <memory>
#include <vector>

GameMap::GameMap(Tilemap& tileMap, std::shared_ptr<Model> quad, std::shared_ptr<Shader> shader) : tilemap(tileMap)
{
	this->quad = quad;
	this->shader = shader;

	InitInstancedMapData();
}

GameMap::~GameMap()
{
	glDeleteBuffers(1,&modelMatVBO);
	glDeleteBuffers(1,&textureVBO);
}

Tilemap& GameMap::GetTilemap()
{
	return tilemap;
}

/*
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

				DrawTile(tile, slopeTilePos, camera, renderer);
				DrawSlope(slopePos, slope, camera, renderer);
			}
			else
			{
				DrawTile(tile, pos, camera, renderer);
			}



		}
	}

}
*/

void GameMap::InitInstancedMapData()
{
	// fill lists
	modelMatrices.resize(tilemap.mapXSize * tilemap.mapZSize);
	textureIds.resize(tilemap.mapXSize * tilemap.mapZSize);

	for (float i = 0; i < tilemap.mapXSize; i++)
	{
		for (float j = 0; j < tilemap.mapZSize; j++)
		{
			auto& tile = tilemap.GetTile(i, j);
			glm::vec3 pos;

			if (tile.signature & Tile_Type::SLOPE)
			{
				pos = glm::vec3(i, tilemap.GetTileHeight(i, j) - 1.5f, j); // slopes tiles are at height + 0.5 logically, but should appear at height visually
			}
			else
			{
				pos = glm::vec3(i, tilemap.GetTileHeight(i, j) - 1.0f, j);
			}

			glm::mat4 modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, pos);
			modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

			modelMatrices[(int)(j * tilemap.mapXSize + i)] = modelMat;
			textureIds[(int)(j * tilemap.mapXSize + i)] = tile.textureIndex;
		}
	}

	// create VBO for modelMatrices
	glGenBuffers(1, &modelMatVBO);

	glBindBuffer(GL_ARRAY_BUFFER, modelMatVBO);
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// create VBO for textureIds
	glGenBuffers(1, &textureVBO);

	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, textureIds.size() * sizeof(float), &textureIds[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// set VAO data
	std::shared_ptr<Mesh> quadMesh = quad->GetMeshes()[0];

	glBindVertexArray(quadMesh->GetVAO());

	glBindBuffer(GL_ARRAY_BUFFER, modelMatVBO);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GameMap::DrawGameMap(Renderer& renderer, Camera& camera)
{
	shader->Bind();

	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	shader->Unbind();

	renderer.RenderModelInstanced(*quad, *shader, tilemap.mapXSize * tilemap.mapZSize);
}

void GameMap::DrawTile(Tile tile, glm::vec3 pos, Camera& camera, Renderer& renderer)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

	shader->Bind();

	shader->setMat4("model", modelMat);
	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	shader->setFloat("texIndex", tile.textureIndex);

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

	shader->setFloat("texIndex", 76.0f); // basic stair texture

	shader->Unbind();

	renderer.RenderModel(*quad, *shader);
}