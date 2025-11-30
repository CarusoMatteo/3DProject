#pragma once

#include <any>
#include <string>

using namespace std;
using namespace glm;

struct Uniform
{
	string name;
	unsigned int location = static_cast<unsigned int>(-1);
	std::any value;
};
#ifdef USE_UBOs
struct UniformValues
{
	mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;
	float creationTime;
	float currentTime;
	fvec2 screenSize;
	bool isVisible;
};
#endif
struct Uniforms
{
	Uniform projectionMatrix = {"projectionMatrix"};
	Uniform modelMatrix = {"modelMatrix"};
	Uniform viewMatrix = {"viewMatrix"};
	Uniform creationTime = {"creationTime"};
	Uniform currentTime = {"currentTime"};
	Uniform screenSize = {"screenSize"};
	Uniform isVisible = {"isVisible"};
#ifdef USE_UBOs
	UniformValues toValues()
	{
		return {
			any_cast<mat4>(projectionMatrix.value),
			any_cast<mat4>(modelMatrix.value),
			any_cast<mat4>(viewMatrix.value),
			any_cast<float>(creationTime.value),
			any_cast<float>(currentTime.value),
			any_cast<fvec2>(screenSize.value),
			any_cast<bool>(isVisible.value)};
	}
#endif
};