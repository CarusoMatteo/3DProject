#pragma once

#include "IGameObject.h"

struct Transform;

class IVisibleGameObject : public IGameObject
{
public:
	virtual void render() = 0;

	virtual Transform getTransform() const = 0;
	virtual void setTransform(const Transform transform) = 0;
};
