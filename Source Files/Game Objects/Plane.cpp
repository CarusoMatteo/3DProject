#include "../../Header Files/Game Objects/Plane.h"
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

using namespace glm;
using namespace std;

Plane::Plane(const shared_ptr<Renderer> renderer) : Plane({fvec3(0, -1.5f, 0), Rotation(), fvec3(5, 1, 5)}, renderer)
{
}

Plane::Plane(const Transform transform, const shared_ptr<Renderer> renderer)
{
	const string name = "Plane";
	const fvec2 size = fvec2(1);

	this->model = ModelFactory::plane(name, size, renderer, transform, TextureFactory::none());
}

void Plane::update(float deltaTime)
{
	return;
}

void Plane::render(const float currentTime)
{
	this->model->render(currentTime);
}

Transform Plane::getTransform() const
{
	return this->model->getTransform();
}

void Plane::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}

vector<shared_ptr<Mesh>> Plane::getMeshes() const
{
	return this->model->getMeshes();
}

string Plane::getName() const
{
	return "Plane";
}
