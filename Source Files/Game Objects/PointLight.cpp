#include "../../Header Files/Game Objects/PointLight.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

unique_ptr<PointLight> PointLight::instance = nullptr;

PointLight *PointLight::I()
{
	if (PointLight::instance == nullptr)
	{
		PointLight::instance = unique_ptr<PointLight>(new PointLight());
	}
	return PointLight::instance.get();
}

PointLight::PointLight()
{
	this->position = fvec3(-10, 10, 10);
	this->color = fvec3(1);
	this->power = 2.0f;
}

void PointLight::update(float deltaTime)
{
}

fvec3 *PointLight::getPositionPtr()
{
	return &this->position;
}

fvec3 PointLight::getPosition() const
{
	return this->position;
}

fvec3 PointLight::getColor() const
{
	return this->color;
}

float PointLight::getPower() const
{
	return this->power;
}
