#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>

Plane::Plane()
{
	const string name = "Plane";
	const fvec2 size = fvec2(1);
	const ShaderFiles shaderFiles = {
		"Shaders/Cube/Vertex.glsl",
		"Shaders/Cube/Fragment.glsl"};

	Transform planeTransform = {
		fvec3(0, -1.5f, 0),
		0.0f,
		fvec3(0.0f),
		fvec3(5000, 0, 5000)};

	this->model = ModelFactory::plane(name, size, shaderFiles, planeTransform);
}

void Plane::update(float deltaTime)
{
}

void Plane::render()
{
	this->model->render();
}
