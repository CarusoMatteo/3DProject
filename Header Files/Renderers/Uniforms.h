#pragma once

#include <glm/glm.hpp>
#include <string>

using namespace std;
using namespace glm;

/// @brief `none` uniforms are those that do not possess a unique value, such as textures samplers.
typedef void *none;

template <typename T>
struct Uniform
{
	string name;
	unsigned int location = static_cast<unsigned int>(-1);
	T value;
};

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

	Uniform<none> texture = {"textureSampler"};
	Uniform<bool> useTexture = {"useTexture"};

	Uniform<none> skybox = {"skyboxSampler"};
};

struct DeferredUniforms
{
	Uniform<none> diffuseTexture1 = {"diffuseTextureSampler1"};
	Uniform<none> specularTexture1 = {"specularTextureSampler1"};

	Uniform<float> creationTime = {"creationTime"};
	Uniform<bool> isVisible = {"isVisible"};
};