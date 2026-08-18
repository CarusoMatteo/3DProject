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

struct ForwardUniforms
{
	Uniform<fmat4> projectionMatrix = {"projectionMatrix"};
	Uniform<fmat4> modelMatrix = {"modelMatrix"};
	Uniform<fmat4> viewMatrix = {"viewMatrix"};
	Uniform<fvec3> viewPosition = {"viewPosition"};

	Uniform<fmat4> projectionMatrixPrev = {"projectionMatrixPrev"};
	Uniform<fmat4> modelMatrixPrev = {"modelMatrixPrev"};
	Uniform<fmat4> viewMatrixPrev = {"viewMatrixPrev"};

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

struct GeometryUniforms
{
	Uniform<fmat4> projectionMatrix = {"projection"};
	Uniform<fmat4> modelMatrix = {"model"};
	Uniform<fmat4> viewMatrix = {"view"};

	Uniform<unsigned int> positions = {"gPosition"};
	Uniform<unsigned int> normals = {"gNormal"};
	Uniform<unsigned int> albedosSpecular = {"gAlbedoSpec"};

	Uniform<none> diffuseTexture1 = {"texture_diffuse1"};
	Uniform<none> specularTexture1 = {"texture_specular1"};

	// Can't create one uniform array because the number of lights is not known at compile time,
	// so we need to create the string constants needed to create them dynamically.
	// Example: "lights[0].Position"
	string lightsStructName = "lights";
	string lightPositionName = "Position";
	string lightColorName = "Color";
	string lightLinearName = "Linear";
	string lightQuadraticName = "Quadratic";
};
