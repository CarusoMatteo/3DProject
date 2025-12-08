#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

Plane::Plane()
{
	const string name = "Plane";
	const fvec2 size = fvec2(1);

	Transform planeTransform = {
		fvec3(0, -1.5f, 0),
		Rotation(),
		fvec3(5000, 0, 5000)};

	this->model = ModelFactory::plane(name, size, ShaderFactory::createUnlitShader(), planeTransform);
}

void Plane::update(float deltaTime)
{
	return;
}

void Plane::render()
{
	this->model->render();
}

Transform Plane::getTransform() const
{
	return this->model->getTransform();
}

void Plane::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}
