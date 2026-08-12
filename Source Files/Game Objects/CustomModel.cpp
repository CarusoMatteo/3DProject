#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Model/Model.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/Renderer.h"
#include "../../Header Files/Texture/Texture.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include "../../Header Files/Texture/TextureLoader.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

CustomModel::CustomModel(const string modelName, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const optional<shared_ptr<Texture>> texture) : CustomModel(modelName, Transform(), rendererFactoryFunction, texture)
{
}

CustomModel::CustomModel(const string modelName, const Transform transform, const function<shared_ptr<Renderer>()> rendererFactoryFunction, const optional<shared_ptr<Texture>> texture) : name(modelName)
{
	this->model = ModelFactory::fromFile(modelName, rendererFactoryFunction, transform, texture);
}

void CustomModel::update(float deltaTime)
{
	return;
}

void CustomModel::render(const float currentTime)
{
	this->model->render(currentTime);
}

Transform CustomModel::getTransform() const
{
	return this->model->getTransform();
}

void CustomModel::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}

vector<shared_ptr<Mesh>> CustomModel::getMeshes() const
{
	return this->model->getMeshes();
}

string CustomModel::getName() const
{
	return this->name;
}
