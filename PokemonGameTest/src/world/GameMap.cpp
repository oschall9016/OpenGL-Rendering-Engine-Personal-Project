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

GameMap::GameMap(Tilemap& tileMap, std::shared_ptr<Texture> tileSheet, std::shared_ptr<Shader> shader) : tilemap(tileMap)
{
	quad = Model::CreateQuad(tileSheet);
	slopeQuad = Model::CreateQuad(tileSheet);
	this->shader = shader;

	InitInstancedMapData();
}

GameMap::~GameMap()
{
	glDeleteBuffers(1,&tileModelMatVBO);
	glDeleteBuffers(1,&textureVBO);
}

Tilemap& GameMap::GetTilemap()
{
	return tilemap;
}

void GameMap::InitInstancedMapData()
{
	// fill lists

	float mapSize = tilemap.mapXSize * tilemap.mapZSize; //TODO: make this a part of tilemap

	tileModelMats.resize(mapSize);
	tileTextureIds.resize(mapSize);

	for (float i = 0; i < tilemap.mapXSize; i++)
	{
		for (float j = 0; j < tilemap.mapZSize; j++)
		{
			auto& tile = tilemap.GetTile(i, j);
			float tileHeight = tilemap.GetTileHeight(i, j);
			int tileIndex = (int)(j * tilemap.mapXSize) + i;

			glm::mat4 modelMat = createTileModelMat(i,j, tileHeight);

			tileModelMats[tileIndex] = modelMat;
			tileTextureIds[tileIndex] = tile.textureIndex;
		}
	}

	//////////////////////// QUAD VAO/VBO ///////////////////////////
	
	// create VBO for slope modelMatrices and textureIds
	glGenBuffers(1, &tileModelMatVBO);
	glGenBuffers(1, &textureVBO);
	createPosTexBuffer(tileModelMatVBO, textureVBO, tileModelMats, tileTextureIds);

	// set tile VAO data
	std::shared_ptr<Mesh> quadMesh = quad->GetMeshes()[0];

	glBindVertexArray(quadMesh->GetVAO());

		// tile model
	glBindBuffer(GL_ARRAY_BUFFER, tileModelMatVBO);

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

		// tile texture
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//////////////////////// SLOPE VAO/VBO ///////////////////////////

	// create VBO for slope modelMatrices and textureIds
	glGenBuffers(1, &slopeModelMatVBO);	
	glGenBuffers(1, &slopeTextureVBO);
	createPosTexBuffer(slopeModelMatVBO, slopeTextureVBO, slopeModelMats, slopeTextureIds);

	// set slope VAO data
	std::shared_ptr<Mesh> slopeMesh = slopeQuad->GetMeshes()[0];

	glBindVertexArray(slopeMesh->GetVAO());

	// slope model
	glBindBuffer(GL_ARRAY_BUFFER, slopeModelMatVBO);

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

	// slope texture
	glBindBuffer(GL_ARRAY_BUFFER, slopeTextureVBO);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GameMap::SetTileHeight(float x, float z, float height)
{
	// update tile logic
	tilemap.SetTileHeight(x, z, height);

	// update tile visuals
	float modelMatOffset = (z * tilemap.mapXSize) + x;

	// update tile vector
	glm::mat4 modelMat = createTileModelMat(x,z,height);
	tileModelMats[modelMatOffset] = modelMat;

	// update tile buffer
	glBindBuffer(GL_ARRAY_BUFFER, tileModelMatVBO);

	glBufferSubData(GL_ARRAY_BUFFER, modelMatOffset * sizeof(glm::mat4), sizeof(glm::mat4), &modelMat);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameMap::SetSlope(float x, float z, bool removeSlope, Rotate_Direction direction)
{
	// update tile logic
	tilemap.SetSlope(x, z, removeSlope, direction);

	// update slope visuals

	// add to vectors
	auto& slope = tilemap.slopes.at((z * tilemap.mapXSize) + x);
	glm::mat4 modelMat = createSlopeModelMat(x,z,tilemap.GetTileHeight(x,z));

	slopeModelMats.push_back(modelMat);
	slopeTextureIds.push_back(slope.textureIndex);


	// remake buffers
	createPosTexBuffer(slopeModelMatVBO,slopeTextureVBO,slopeModelMats,slopeTextureIds);
}

void GameMap::DrawGameMap(Renderer& renderer, Camera& camera)
{
	shader->Bind();

	shader->setMat4("view", camera.GetViewMatrix());
	shader->setMat4("projection", camera.GetProjectionMatrix());

	shader->Unbind();

	renderer.RenderModelInstanced(*quad, *shader, tilemap.mapXSize * tilemap.mapZSize);
	renderer.RenderModelInstanced(*slopeQuad, *shader, tilemap.slopes.size());
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

glm::mat4 GameMap::createTileModelMat(float x, float z, float height)
{
	auto& tile = tilemap.GetTile(x, z);
	glm::vec3 pos;

	if (tile.signature & Tile_Type::SLOPE)
	{
		pos = glm::vec3(x, height - 1.5f, z); // slopes tiles are at height + 0.5 logically, but should appear at height visually
	}
	else
	{
		pos = glm::vec3(x, height - 1.0f, z);
	}

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

	return modelMat;

	
}

glm::mat4 GameMap::createSlopeModelMat(float x, float z, float height)
{
	glm::vec3 pos;

	pos = glm::vec3(x, height - 1.0f, z); // slopes tiles are at height + 0.5 logically, but should appear at height visually

	auto& slope = tilemap.slopes.at((z * tilemap.mapXSize) + x);
	float direction = static_cast<float>(slope.direction);

	float scaleFactor = std::sqrtf(2.0f);
	float slopeAngle = std::atan(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::rotate(modelMat, glm::radians(direction), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, slopeAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::scale(modelMat, glm::vec3(1.0f, scaleFactor, 1.0f));

	return modelMat;
}

void GameMap::createPosTexBuffer(unsigned int& posVBO, unsigned int& texVBO, std::vector<glm::mat4>& posMats, std::vector<float>& texIDs)
{
	const void* posData = (posMats.size() == 0) ? nullptr : &posMats[0];
	const void* texData = (texIDs.size() == 0) ? nullptr : &texIDs[0];

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, posMats.size() * sizeof(glm::mat4), posData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, texIDs.size() * sizeof(float), texData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}