#include "SpriteAnimation.h"

const int SpriteAnimation::GetCurrentFrame()
{
	return frames[currFrameIndex];
}

void SpriteAnimation::SetCurrentFrame(int frame)
{
	currFrameIndex = frame;
}

void SpriteAnimation::Update()
{
	if (currFrameIndex + 1 >= frames.size()) currFrameIndex = 0;
	else currFrameIndex++;
}