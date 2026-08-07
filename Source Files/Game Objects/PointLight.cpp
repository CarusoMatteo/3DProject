#include "../../Header Files/Game Objects/PointLight.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>

using namespace glm;
using namespace std;

optional<unique_ptr<PointLight>> PointLight::instance = nullopt;

PointLight *PointLight::I()
{
	if (!PointLight::instance.has_value())
	{
		PointLight::instance = unique_ptr<PointLight>(new PointLight());
	}
	return PointLight::instance.value().get();
}

PointLight::PointLight()
{
	this->position = fvec3(-10, 10, 10);
	this->color = fvec3(1);
	this->power = 2.0f;
}

void PointLight::update(float deltaTime)
{
	return;
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
