#pragma once

#include "IGameObject.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Mesh;

class PointLight : public IGameObject
{
public:
	PointLight();
	~PointLight() = default;

	void update(const float deltaTime) override;

private:
	const fvec3 position = fvec3(1, 0, 50);
	const fvec3 color = fvec3(1);
	const float power = 2.0f;
};
