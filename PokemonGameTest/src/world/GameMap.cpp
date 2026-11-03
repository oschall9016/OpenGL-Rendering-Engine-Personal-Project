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
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

GameMap::GameMap(Tilemap& tileMap, std::shared_ptr<Texture> tileSheet) : tilemap(tileMap)
{
	quad = Model::CreateQuad(tileSheet);
	slopeQuad = Model::CreateQuad(tileSheet);

	tileShader = std::make_shared<Shader>("assets/shaders/v_tile.vs", "assets/shaders/f_tile.fs");
	modelShader = std::make_shared<Shader>("assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");

	InitInstancedMapData();
}

GameMap::GameMap(Tilemap& tileMap, std::shared_ptr<Texture> tileSheet, std::string mapPath) : tilemap(tileMap)
{
	quad = Model::CreateQuad(tileSheet);
	slopeQuad = Model::CreateQuad(tileSheet);

	tileShader = std::make_shared<Shader>("assets/shaders/v_tile.vs", "assets/shaders/f_tile.fs");
	modelShader = std::make_shared<Shader>("assets/shaders/BasicBackpackVertex.vs", "assets/shaders/BasicBackpackFragment.fs");

	ReadMapFromFile(mapPath);

	InitInstancedMapData();
}

void GameMap::ReadMapFromFile(std::string mapPath)
{
	std::ifstream mapFile(mapPath);

	if (!mapFile.is_open())
	{
		std::cout << "COULD NOT OPEN FILE: " + mapPath + "\n";
		return;
	}

	// loop over each tile
	std::string tileString;
	int tileIndex = 0;
	while (mapFile >> tileString)
	{
		float tileX = std::fmod(tileIndex, tilemap.mapXSize);
		float tileZ = std::floor(tileIndex / tilemap.mapXSize);

		// split the string into 3 seperate options
		// tex id , height , type
		std::vector<std::string> tileOptions;

		std::string splitString;
		std::stringstream tileStream(tileString);
		while (getline(tileStream, splitString, ','))
		{
			tileOptions.push_back(splitString);
		}

		// set texture
		tilemap.SetTileTextureIndex(tileX, tileZ, std::stoi(tileOptions[0]));

		// set height
		tilemap.SetTileHeight(tileX, tileZ, std::stoi(tileOptions[1]));

		// set type
		// W = walkable (do nothing since tiles are walkable by default)
		// C = collider
		// U/D/L/R = slope + direction
		// warp will be handled elsewhere due to the extra complexity
		switch (tileOptions[2][0]) // only one character
		{
		case 'C':
			//tilemap.SetCollider
			break;
		case 'U':
			SetSlope(tileX, tileZ, false, Rotate_Direction::UP);
			break;
		case 'D':
			SetSlope(tileX, tileZ, false, Rotate_Direction::DOWN);
			break;
		case 'L':
			SetSlope(tileX, tileZ, false, Rotate_Direction::LEFT);
			break;
		case 'R':
			SetSlope(tileX, tileZ, false, Rotate_Direction::RIGHT);
			break;
		}
		tileIndex++;
	}
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

	if (modelMatOffset < tileModelMats.size())
	{
		// update tile vector
		glm::mat4 modelMat = createTileModelMat(x,z,height);
		tileModelMats[modelMatOffset] = modelMat;

		// update tile buffer
		glBindBuffer(GL_ARRAY_BUFFER, tileModelMatVBO);

		glBufferSubData(GL_ARRAY_BUFFER, modelMatOffset * sizeof(glm::mat4), sizeof(glm::mat4), &modelMat);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
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
	// render tiles and slopes
	tileShader->Bind();

	tileShader->setMat4("view", camera.GetViewMatrix());
	tileShader->setMat4("projection", camera.GetProjectionMatrix());

	tileShader->Unbind();

	renderer.RenderModelInstanced(*quad, *tileShader, tilemap.mapXSize * tilemap.mapZSize);
	renderer.RenderModelInstanced(*slopeQuad, *tileShader, tilemap.slopes.size());

	// render models
	modelShader->Bind();

	modelShader->setMat4("view", camera.GetViewMatrix());
	modelShader->setMat4("projection", camera.GetProjectionMatrix());

	modelShader->Unbind();

	for (int i = 0; i < mapModels.size(); i++)
	{
		auto position = mapModelTransforms[i].position;
		auto rotation = mapModelTransforms[i].rotation;
		auto scale = mapModelTransforms[i].scale;

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, position);
		modelMat *= glm::mat4_cast(rotation);
		modelMat = glm::scale(modelMat, scale);

		modelShader->Bind();

		modelShader->setMat4("model", modelMat);

		modelShader->Unbind();

		renderer.RenderModel(*mapModels[i], *modelShader);
	}

	
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

void GameMap::AddMapModel(std::shared_ptr<Model> model, component_Transform transform)
{
	mapModels.push_back(model);
	mapModelTransforms.push_back(transform);
}