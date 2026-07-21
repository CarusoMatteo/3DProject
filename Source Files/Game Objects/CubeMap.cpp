#include "../../Header Files/Game Objects/CubeMap.h"

#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Model/CubeMapMesh.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Model/Transform.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

unique_ptr<CubeMap> CubeMap::instance = nullptr;

CubeMap *CubeMap::I()
{
	if (CubeMap::instance == nullptr)
	{
		CubeMap::instance = unique_ptr<CubeMap>(new CubeMap());
	}
	return CubeMap::instance.get();
}

CubeMap::CubeMap()
{
	const string name = "CubeMap";
	const float length = 1.0f;

	this->model = ModelFactory::cubeMap(name, length, ShaderFactory::createCubeMapShader(), Transform());
}

void CubeMap::update(float deltaTime)
{
}

void CubeMap::render()
{
	this->model->render();
}

Transform CubeMap::getTransform() const
{
	return this->model->getTransform();
}

void CubeMap::setTransform(const Transform transform)
{
	this->model->setTransform(transform);
}

vector<shared_ptr<Mesh>> CubeMap::getMeshes() const
{
	throw runtime_error("CubeMap::getMeshes() is not implemented. Use CubeMap::getMesh() instead.");
}

shared_ptr<CubeMapMesh> CubeMap::getMesh() const
{
	return this->model->getMesh();
}

string CubeMap::getName() const
{
	return "CubeMap";
}

vector<string> CubeMap::getFacesPaths(string path)
{
	return {
		path + "posx.jpg",
		path + "negx.jpg",
		path + "posy.jpg",
		path + "negy.jpg",
		path + "posz.jpg",
		path + "negz.jpg"};

	/*
	return {
		path + "right.jpg",
		path + "left.jpg",
		path + "top.jpg",
		path + "bottom.jpg",
		path + "front.jpg",
		path + "back.jpg"};
	*/
}
