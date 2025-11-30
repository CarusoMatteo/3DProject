#pragma once

#include <glm/glm.hpp>
#include <string>

using namespace std;
using namespace glm;

template <typename T>
struct Uniform
{
	string name;
	unsigned int location = static_cast<unsigned int>(-1);
	T value;
};

#ifdef USE_UBOs
struct UniformValues
{
	fmat4 projectionMatrix;
	fmat4 modelMatrix;
	fmat4 viewMatrix;
	float creationTime;
	float currentTime;
	ivec2 screenSize;
	int isVisible;
};
#endif

struct Uniforms
{
	Uniform<fmat4> projectionMatrix = {"projectionMatrix"};
	Uniform<fmat4> modelMatrix = {"modelMatrix"};
	Uniform<fmat4> viewMatrix = {"viewMatrix"};
	Uniform<float> creationTime = {"creationTime"};
	Uniform<float> currentTime = {"currentTime"};
	Uniform<ivec2> screenSize = {"screenSize"};
	Uniform<bool> isVisible = {"isVisible"};
#ifdef USE_UBOs
	UniformValues toValues()
	{
		return {
			projectionMatrix.value,
			modelMatrix.value,
			viewMatrix.value,
			creationTime.value,
			currentTime.value,
			screenSize.value,
			isVisible.value};
	}
#endif
};
