#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include <memory>

Cube::Cube()
{
	const float length = 1.0f;
	const ShaderFiles shaderFiles = {
		"Shaders/Cube/Vertex.glsl",
		"Shaders/Cube/Fragment.glsl"};

	this->model = ModelFactory::cube(length, shaderFiles);
}

void Cube::update(float deltaTime)
{
}

void Cube::render()
{
}
