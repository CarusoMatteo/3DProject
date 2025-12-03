#pragma once

#include "../Model/Model.h"
#include "../Model/Transform.h"
#include "IVisibleGameObject.h"
#include <memory>

using namespace std;

class Mesh;

class Sphere : public IVisibleGameObject
{
public:
	Sphere();
	~Sphere() = default;

	void update(const float deltaTime) override;
	void render() override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;

private:
	shared_ptr<Model> model;
};
