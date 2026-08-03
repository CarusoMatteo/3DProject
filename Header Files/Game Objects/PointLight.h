#pragma once

#include "IGameObject.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>

using namespace std;
using namespace glm;

class Mesh;

class PointLight : public IGameObject
{
public:
	// Singleton access.
	static PointLight *I();

	~PointLight() = default;

	void update(const float deltaTime) override;

	fvec3 getPosition() const;
	fvec3 *getPositionPtr();
	fvec3 getColor() const;
	float getPower() const;

private:
	static optional<unique_ptr<PointLight>> instance;

	PointLight();

	fvec3 position;
	fvec3 color;
	float power;
};
