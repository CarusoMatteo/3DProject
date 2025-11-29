#pragma once
#include "IGameObject.h"

class ITemporaryGameObject : public IGameObject
{
public:
	virtual bool shouldDelete() = 0;
};
