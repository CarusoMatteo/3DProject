#pragma once

#include "IGameObject.h"

class IVisibleGameObject : public IGameObject
{
public:
	virtual void render() = 0;
};
