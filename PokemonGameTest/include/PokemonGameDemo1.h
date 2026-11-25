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
	void InitObjects();
	void SetupMap();
	void InitECS();
	void PlaceMapModels();
	void RunRenderLoop();
	void SetCameraPos();

	//SDLManager sdlManager;
	//Camera camera;
	//DeltaTime dt;
	AssetManager aManager;
	//EntityComponentSystem ecs;
	GameWorld world;

};