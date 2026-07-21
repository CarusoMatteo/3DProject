#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/CubeMapMesh.h"
#include "../../Header Files/Model/CubeMapModel.h"
#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace std;
using namespace glm;

shared_ptr<Model> ModelFactory::plane(const string name, const fvec2 size, const shared_ptr<Shader> shader, Transform modelTransform)
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

	shared_ptr<Mesh> planeMesh = MeshFactory::plane(name, size, shader, planeTransform);

	return shared_ptr<Model>(new Model(modelTransform, {planeMesh}));
}

shared_ptr<Model> ModelFactory::cube(const string name, const float length, const shared_ptr<Shader> shader, Transform modelTransform)
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

	shared_ptr<Mesh> cubeMesh = MeshFactory::cube(name, length, shader, cubeTransform);

	return shared_ptr<Model>(new Model(modelTransform, {cubeMesh}));
}

shared_ptr<Model> ModelFactory::sphere(const string name, const fvec3 radius, const shared_ptr<Shader> shader, Transform modelTransform)
{
	const fvec3 spherePosition(0.0f);
	const float sphereRotation = 0.0f;
	const fvec3 sphereRotationAxis(0.0f);

	Transform sphereTransform = {
		spherePosition,
		sphereRotation,
		sphereRotationAxis,
		radius};

	shared_ptr<Mesh> sphereMesh = MeshFactory::sphere(name, radius, shader, sphereTransform);

	return shared_ptr<Model>(new Model(modelTransform, {sphereMesh}));
}

shared_ptr<CubeMapModel> ModelFactory::cubeMap(const string name, const float length, const shared_ptr<Shader> shader, Transform modelTransform)
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

	shared_ptr<CubeMapMesh> cubeMapMesh = MeshFactory::cubeMap(name, length, shader, cubeTransform);

	return shared_ptr<CubeMapModel>(new CubeMapModel(modelTransform, cubeMapMesh));
}
