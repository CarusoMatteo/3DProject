#pragma once

#include "../Texture/Texture.h"
#include "IVisibleGameObject.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

class Mesh;
class Model;
class Renderer;
struct Transform;

class CustomModel : public IVisibleGameObject
{
public:
	CustomModel(const string modelName, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const optional<shared_ptr<Texture>> texture);
	CustomModel(const string modelName, const Transform transform, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const optional<shared_ptr<Texture>> texture);
	~CustomModel() = default;

	void update(const float deltaTime) override;
	void render(const float currentTime) override;

	Transform getTransform() const override;
	void setTransform(const Transform transform) override;
	vector<shared_ptr<Mesh>> getMeshes() const override;
	string getName() const override;

private:
	shared_ptr<Model> model;
	const string name;
};
