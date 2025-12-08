#pragma once

#include "../Game Objects/IVisibleGameObject.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace std;
using namespace glm;

class IGui
{
public:
	virtual ~IGui() = default;
	virtual void drawGui(vector<shared_ptr<IVisibleGameObject>> objects) = 0;
	virtual const shared_ptr<fvec3> getClearColor() = 0;
};
