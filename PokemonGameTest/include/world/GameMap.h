#pragma once

#include "Tilemap.h"
#include "Slope.h"

#include "Texture.h"
#include "Shader.h"

#include "Renderer.h"
#include "Camera.h"

#include <vector>
#include <memory>

class GameMap
{
public:
	GameMap(Tilemap& tileMap, std::shared_ptr<Texture> tileSheet, std::shared_ptr<Shader> shader);
	~GameMap();

	void InitInstancedMapData();

	Tilemap& GetTilemap();

	void SetTileHeight(float x, float z, float height);
	void SetSlope(float x, float z, bool removeSlope, Rotate_Direction direction = Rotate_Direction::UP);

	void DrawGameMap(Renderer& renderer, Camera& camera);

	void DrawSlope(glm::vec3 pos, Slope slope, Camera& camera, Renderer& renderer);

private:

	glm::mat4 createTileModelMat(float x, float z, float height);
	glm::mat4 createSlopeModelMat(float x, float z, float height);

	void createPosTexBuffer(unsigned int& posVBO, unsigned int& texVBO, std::vector<glm::mat4>& posMats, std::vector<float>& texIDs);

	Tilemap& tilemap;
	
	std::shared_ptr<Model> quad;
	std::shared_ptr<Model> slopeQuad;
	std::shared_ptr<Shader> shader;
	
	std::vector<glm::mat4> tileModelMats;
	std::vector<float> tileTextureIds;

	std::vector<glm::mat4> slopeModelMats;
	std::vector<float> slopeTextureIds;

	unsigned int tileModelMatVBO;
	unsigned int textureVBO;

	unsigned int slopeModelMatVBO;
	unsigned int slopeTextureVBO;

};