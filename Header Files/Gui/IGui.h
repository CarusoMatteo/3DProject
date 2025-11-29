#pragma once

#include <glm/glm.hpp>
#include <memory>

using namespace std;
using namespace glm;

class IGui
{
public:
	virtual ~IGui() = default;
	virtual void drawGui() = 0;
	virtual const shared_ptr<fvec3> getClearColor() = 0;
};
