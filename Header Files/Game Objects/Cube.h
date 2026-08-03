#pragma once

#include "IVisibleGameObject.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
class Model;
class Renderer;
struct Transform;

class Cube : public IVisibleGameObject
{
public:
	Cube(const shared_ptr<Renderer> renderer);
	Cube(const Transform transform, const shared_ptr<Renderer> renderer);
	~Cube() = default;

	void update(const float deltaTime) override;
	void render() override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;
	vector<shared_ptr<Mesh>> getMeshes() const override;
	string getName() const override;

private:
	shared_ptr<Model> model;
};
