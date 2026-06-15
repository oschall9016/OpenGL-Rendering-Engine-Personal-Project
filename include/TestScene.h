#pragma once

#include "Entity.h"
#include "EntityHandler.h"
#include "c_Renderable.h"
#include "SparseSet.h"

class TestScene
{
public:
	TestScene(int maxSize);
	SparseSet<c_Renderable> renderComponentList;
};

inline TestScene::TestScene(int maxSize) : renderComponentList(maxSize)
{}