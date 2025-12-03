#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace std;
using namespace glm;

Sphere::Sphere()
{
	const string name = "Sphere";
	const fvec3 radius = fvec3(1.0f);
	const ShaderFiles shaderFiles = {
		"Shaders/Phong/Phong.vert",
		"Shaders/Phong/Phong.frag"};

	this->model = ModelFactory::sphere(name, radius, shaderFiles, Transform());
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
