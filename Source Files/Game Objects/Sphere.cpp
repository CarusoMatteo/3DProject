#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFactory.h"
#include "../../Header Files/Model/TextureFactory.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Sphere::Sphere(const RendererMode rendererMode)
{
	const string name = "Sphere";
	const fvec3 radius = fvec3(1.0f);
	this->rendererMode = rendererMode;

	this->model = ModelFactory::sphere(name, radius, ShaderFactory::reflection(), Transform(), TextureFactory::brick());
}

void Sphere::update(float deltaTime)
{
}

void Sphere::render()
{
	this->model->render();
}

Transform Sphere::getTransform() const
{
	return this->model->getTransform();
}

void Sphere::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}

vector<shared_ptr<Mesh>> Sphere::getMeshes() const
{
	return this->model->getMeshes();
}

string Sphere::getName() const
{
	return "Sphere";
}

RendererMode Sphere::getRendererMode() const
{
	return this->rendererMode;
}
