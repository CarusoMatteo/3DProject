#include "../../Header Files/Model/MeshFactory.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>

using namespace std;
using namespace glm;

shared_ptr<Model> ModelFactory::cube(float length, ShaderFiles files)
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

	shared_ptr<Mesh> cubeMesh = MeshFactory::cube(length, files, cubeTransform);

	return shared_ptr<Model>(new Model({cubeMesh}));
}
