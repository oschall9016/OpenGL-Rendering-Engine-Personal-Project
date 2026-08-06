#pragma once

#include "Tilemap.h"

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

	void DrawGameMap(Renderer& renderer, Camera& camera);

private:

	Tilemap& tileMap;
	
	std::shared_ptr<Model> quad;
	std::shared_ptr<Shader> shader;

	
};