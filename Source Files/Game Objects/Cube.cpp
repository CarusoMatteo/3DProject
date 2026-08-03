#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFactory.h"
#include "../../Header Files/Model/TextureFactory.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Cube::Cube(const RendererMode rendererMode)
{
	const string name = "Cube";
	const float length = 2.0f;
	this->rendererMode = rendererMode;

	this->model = ModelFactory::cube(name, length, ShaderFactory::phong(), Transform(), TextureFactory::cement());
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

vector<shared_ptr<Mesh>> Cube::getMeshes() const
{
	return this->model->getMeshes();
}

string Cube::getName() const
{
	return "Cube";
}

RendererMode Cube::getRendererMode() const
{
	return this->rendererMode;
}
