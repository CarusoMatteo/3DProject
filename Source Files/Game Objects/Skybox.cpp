#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Renderer.h"
#include "../../Header Files/Texture/CubeMapTextureFactory.h"
#include <memory>
#include <string>
#include <vector>

Skybox::Skybox(const Transform transform, const shared_ptr<Renderer> renderer)
{
	const string name = "Skybox";
	const float length = 1.0f;

	this->model = ModelFactory::cube(name, length, renderer, transform, CubeMapTextureFactory::clouds());
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
