#include "GameWorld.h"


#include "Renderer.h"
#include "Camera.h"

GameWorld::GameWorld() {}

void GameWorld::DrawCurrentMap(Renderer& renderer, Camera& camera)
{
	currentMap->DrawGameMap(renderer, camera);
}