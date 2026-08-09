#pragma once

#include <glm/glm.hpp>

using namespace glm;

struct LightValue
{
	fvec3 position = fvec3(0);
	fvec3 color = fvec3(1);
	// Attenuation parameters for radius calculation.
	float linear = 0.7f;
	float quadratic = 1.8f;
};