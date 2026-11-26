#pragma once

#include <vector>

class SpriteAnimation
{
public:
	void Update();
	int GetCurrentFrame();
	void SetCurrentFrame(int frame);

protected:
	std::vector<int> frames;
	int currFrameIndex = 0;
};