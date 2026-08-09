#pragma once

#include "IGameObject.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
struct Transform;

class IVisibleGameObject : public IGameObject
{
public:
	virtual void render(const float currentTime) = 0;

	virtual Transform getTransform() const = 0;
	virtual void setTransform(const Transform transform) = 0;
	virtual vector<shared_ptr<Mesh>> getMeshes() const = 0;
	virtual string getName() const = 0;
};
