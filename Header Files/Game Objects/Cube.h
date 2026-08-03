#pragma once

#include "../Model/Model.h"
#include "IVisibleGameObject.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
struct Transform;

class Cube : public IVisibleGameObject
{
public:
	Cube(const RendererMode rendererMode);
	~Cube() = default;

	void update(const float deltaTime) override;
	void render() override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;
	vector<shared_ptr<Mesh>> getMeshes() const override;
	string getName() const override;

	RendererMode getRendererMode() const override;

private:
	shared_ptr<Model> model;
};
