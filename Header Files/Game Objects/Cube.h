#pragma once

#include "../Model/Model.h"
#include "IVisibleGameObject.h"
#include <memory>

using namespace std;

class Mesh;
struct Transform;

class Cube : public IVisibleGameObject
{
public:
	Cube();
	~Cube() = default;

	void update(const float deltaTime) override;
	void render() override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;

private:
	shared_ptr<Model> model;
};
