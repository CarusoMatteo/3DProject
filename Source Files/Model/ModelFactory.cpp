#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace std;
using namespace glm;

shared_ptr<Model> ModelFactory::plane(const string name, const fvec2 size, const ShaderFiles files, Transform modelTransform)
{
	const fvec3 planePosition(0.0f);
	const float planeRotation = 0.0f;
	const fvec3 planeRotationAxis(0.0f);
	const fvec3 planeScale(50.0f);

	Transform planeTransform = {
		planePosition,
		planeRotation,
		planeRotationAxis,
		planeScale};

	shared_ptr<Mesh> planeMesh = MeshFactory::plane(name, size, files, planeTransform);

	return shared_ptr<Model>(new Model(modelTransform, {planeMesh}));
}

shared_ptr<Model> ModelFactory::cube(const string name, const float length, const ShaderFiles files, Transform modelTransform)
{
	const fvec3 cubePosition(0.0f);
	const float cubeRotation = 0.0f;
	const fvec3 cubeRotationAxis(0.0f);
	const fvec3 cubeScale(1.0f);

	Transform cubeTransform = {
		cubePosition,
		cubeRotation,
		cubeRotationAxis,
		cubeScale};

	shared_ptr<Mesh> cubeMesh = MeshFactory::cube(name, length, files, cubeTransform);

	return shared_ptr<Model>(new Model(modelTransform, {cubeMesh}));
}
