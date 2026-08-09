#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Lights/LightValue.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

PointLight::PointLight(const LightValue value, const float power)
{
	this->values = value;
	this->power = power;
}

void PointLight::update(float deltaTime)
{
	return;
}

LightValue PointLight::getValues() const
{
	return this->values;
}

fvec3 *PointLight::getPositionPtr()
{
	return &this->values.position;
}

float PointLight::getPower() const
{
	return this->power;
}
