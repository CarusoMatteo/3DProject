#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Random.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <vector>

using namespace glm;
using namespace std;

optional<unique_ptr<LightManager>> LightManager::instance = nullopt;

LightManager *LightManager::I()
{
	return LightManager::I({{.position = fvec3(45), .color = fvec3(1)}});
}

LightManager *LightManager::I(vector<LightValue> lightValues)
{
	if (!LightManager::instance.has_value())
	{
		LightManager::instance = unique_ptr<LightManager>(new LightManager(lightValues));
	}
	return LightManager::instance.value().get();
}

vector<LightValue> LightManager::getValues() const
{
	vector<LightValue> result;
	result.reserve(this->lights.size());
	for (const auto &light : this->lights)
	{
		result.push_back(light->getValues());
	}
	return result;
}

void LightManager::updateLights(const float deltaTime)
{
	for (const auto &light : this->lights)
	{
		light->update(deltaTime);
	}
}

vector<fvec3 *> LightManager::getPositionPtrs()
{
	vector<fvec3 *> result;
	result.reserve(this->lights.size());
	for (const auto &light : this->lights)
	{
		result.push_back(light->getPositionPtr());
	}
	return result;
}

vector<float> LightManager::getPowers() const
{
	vector<float> result;
	result.reserve(this->lights.size());
	for (const auto &light : this->lights)
	{
		result.push_back(light->getPower());
	}
	return result;
}

LightManager::LightManager(vector<LightValue> lightValues)
{
	for (const auto &value : lightValues)
	{
		this->lights.push_back(make_unique<PointLight>(value, 2.0f));
	}
}
