#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Texture.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Sphere::Sphere()
{
	const string name = "Sphere";
	const fvec3 radius = fvec3(1.0f);

	this->model = ModelFactory::sphere(name, radius, ShaderFactory::phong(), Transform(), TextureFactory::brick());
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
