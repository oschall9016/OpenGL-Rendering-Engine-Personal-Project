#pragma once

class DeltaTime
{
public:
	DeltaTime();

	const float Get();
	void Update();

private:
	float currentFrame;
	float deltaTime;
	float lastFrame;
};