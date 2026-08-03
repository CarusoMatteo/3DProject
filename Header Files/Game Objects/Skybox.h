#pragma once

#include "../Model/Transform.h"
#include "../Renderers/ShaderFactory.h"
#include "IVisibleGameObject.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
class Model;
class Renderer;

class Skybox : public IVisibleGameObject
{
public:
	Skybox(const Transform transform = Transform(), const shared_ptr<Renderer> renderer = ShaderFactory::cubeMap());
	~Skybox() = default;

	void update(const float deltaTime) override;
	void render() override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;
	vector<shared_ptr<Mesh>> getMeshes() const override;
	string getName() const override;

private:
	shared_ptr<Model> model;
};
