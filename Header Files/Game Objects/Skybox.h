#pragma once

#include "IVisibleGameObject.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
class Model;
struct Transform;

class Skybox : public IVisibleGameObject
{
public:
	Skybox(const RendererMode rendererMode);
	~Skybox() = default;

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
