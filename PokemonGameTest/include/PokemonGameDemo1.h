#pragma once

#include "SDLManager.h"
#include "AssetManager.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "GameWorld.h"
#include "EntityComponentSystem.h"

class PokemonGameDemo1
{
public:
	PokemonGameDemo1();
	void RunDemo();
private:
	void SetupMap();
	void InitECS();
	void PlaceMapModels();
	void RunRenderLoop();
	void SetCameraPos();

	SDLManager sdlManager;
	Camera camera;
	DeltaTime dt;
	AssetManager aManager;
	EntityComponentSystem ecs;
	GameWorld world;
	Renderer renderer;

	// window settings
	int windowWidth;
	int windowHeight;

	int renderWidth;
	int renderHeight;

	// player settings
	int playerSpawnPosX;
	int playerSpawnPosZ;
	float playerSpawnPosY;

	float playerScale;

};