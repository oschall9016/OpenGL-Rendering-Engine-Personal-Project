#pragma once

#include "Tilemap.h"
#include "Slope.h"

#include "Model.h"
#include "Shader.h"

#include "Renderer.h"
#include "Camera.h"

#include <vector>
#include <memory>

class GameMap
{
public:
	GameMap(Tilemap& tileMap, std::shared_ptr<Model> quad, std::shared_ptr<Shader> shader);

	Tilemap& GetTilemap();

	void DrawGameMap(Renderer& renderer, Camera& camera);

	void DrawTile(glm::vec3 pos, Camera& camera, Renderer& renderer);
	void DrawSlope(glm::vec3 pos, Slope slope, Camera& camera, Renderer& renderer);

private:

	Tilemap& tilemap;
	
	std::shared_ptr<Model> quad;
	std::shared_ptr<Shader> shader;
	
};