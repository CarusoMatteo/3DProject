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
	fvec3 cameraPosition;
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
	Uniform<fvec3> viewPosition = {"viewPosition"};

	Uniform<fvec3> lightPosition = {"light.position"};
	Uniform<fvec3> lightColor = {"light.color"};
	Uniform<float> lightPower = {"light.power"};

	Uniform<fvec3> materialAmbient = {"material.ambient"};
	Uniform<fvec3> materialDiffuse = {"material.diffuse"};
	Uniform<fvec3> materialSpecular = {"material.specular"};
	Uniform<float> materialShininess = {"material.shininess"};

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
			cameraPosition.value,
			creationTime.value,
			currentTime.value,
			screenSize.value,
			isVisible.value};
	}
#endif
};
