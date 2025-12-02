#include "../../Header Files/Game Objects/SimpleObject.h"
#include "../../Header Files/Model/ModelFactory.h"
#include "../../Header Files/Model/ShaderFiles.h"
#include "../../Header Files/Model/Transform.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace std;
using namespace glm;

SimpleObject::SimpleObject()
{
	const string name = "SimpleObject";
	const fvec3 radius = fvec3(1.0f);
	const ShaderFiles shaderFiles = {
		"Shaders/Light/Light.vert",
		"Shaders/Light/Light.frag"};

	this->model = ModelFactory::sphere(name, radius, shaderFiles, Transform());
}

void SimpleObject::update(float deltaTime)
{
}

void SimpleObject::render()
{
	this->model->render();
}
