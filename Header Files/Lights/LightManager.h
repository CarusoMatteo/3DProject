#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <vector>

using namespace glm;
using namespace std;

class PointLight;
struct LightValue;

class LightManager
{
public:
	// Singleton access.
	static LightManager *I();

	~LightManager() = default;

	void updateLights(const float deltaTime);

	vector<LightValue> getValues() const;
	vector<fvec3 *> getPositionPtrs();
	vector<float> getPowers() const;

private:
	static optional<unique_ptr<LightManager>> instance;
	const unsigned int numberOfLights = 1;

	vector<unique_ptr<PointLight>> lights;

	LightManager();
};
