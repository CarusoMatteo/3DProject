#pragma once

#include "../Lights/LightValue.h"
#include "IGameObject.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Mesh;

class PointLight : public IGameObject
{
public:
	PointLight(const LightValue value, const float power);
	~PointLight() = default;

	void update(const float deltaTime) override;

	LightValue getValues() const;
	fvec3 *getPositionPtr();
	float getPower() const;

private:
	LightValue values;
	// Unused in forward rendering in favours of linear and quadratic attenuation parameters in PointLightValue.
	float power;
};