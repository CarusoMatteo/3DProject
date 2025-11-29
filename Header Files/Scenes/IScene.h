#pragma once

class IScene
{
public:
	virtual void updateGameObjects(const float deltaTime) = 0;
	virtual void renderScene() = 0;
};