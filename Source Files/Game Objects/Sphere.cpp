#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Renderer.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Sphere::Sphere(const shared_ptr<Renderer> renderer) : Sphere(Transform(), renderer)
{
}

Sphere::Sphere(const Transform transform, const shared_ptr<Renderer> renderer)
{
	const string name = "Sphere";
	const fvec3 radius = fvec3(1.0f);

	this->model = ModelFactory::sphere(name, radius, renderer, transform, TextureFactory::brick());
}

void Sphere::update(float deltaTime)
{
	return;
}

void Sphere::render(const float currentTime)
{
	this->model->render(currentTime);
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
