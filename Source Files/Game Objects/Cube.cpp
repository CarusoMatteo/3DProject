#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

Cube::Cube()
{
	const string name = "Cube";
	const float length = 1.0f;
	const ShaderFiles shaderFiles = {
		"Shaders/Default/Default.vert",
		"Shaders/Default/Default.frag"};

	this->model = ModelFactory::cube(name, length, shaderFiles, Transform());
}

void Cube::update(float deltaTime)
{
}

void Cube::render()
{
	this->model->render();
}
