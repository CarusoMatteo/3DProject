#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Model/CubeMapTextureFactory.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFactory.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <string>
#include <vector>

Skybox::Skybox()
{
	const string name = "Skybox";
	const float length = 1.0f;

	this->model = ModelFactory::cube(name, length, ShaderFactory::cubeMap(), Transform(), CubeMapTextureFactory::clouds());
}

void Skybox::update(const float deltaTime)
{
}

void Skybox::render()
{
	this->model->render();
}

Transform Skybox::getTransform() const
{
	return this->model->getTransform();
}

void Skybox::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}

vector<shared_ptr<Mesh>> Skybox::getMeshes() const
{
	return this->model->getMeshes();
}

string Skybox::getName() const
{
	return "Skybox";
}
