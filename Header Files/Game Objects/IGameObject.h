#pragma once

class IGameObject
{
public:
	virtual void update(const float deltaTime) = 0;
};
