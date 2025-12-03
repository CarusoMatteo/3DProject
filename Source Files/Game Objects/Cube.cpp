#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>

using namespace std;
using namespace glm;

Cube::Cube()
{
	const string name = "Cube";
	const float length = 2.0f;
	const ShaderFiles shaderFiles = {
		"Shaders/Phong/Phong.vert",
		"Shaders/Phong/Phong.frag"};

	this->model = ModelFactory::cube(name, length, shaderFiles, Transform());
}

void Cube::update(float deltaTime)
{
}

void Cube::render()
{
	this->model->render();
}

Transform Cube::getTransform() const
{
	return this->model->getTransform();
}

void Cube::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}
