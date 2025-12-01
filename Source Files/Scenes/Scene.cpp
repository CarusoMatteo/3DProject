#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Scenes/Scene.h"
#include <glm/glm.hpp>
#include <memory>

Scene::Scene(const shared_ptr<fvec3> clearColor)
{
	Camera::I();
	this->plane = unique_ptr<Plane>(new Plane());
	this->cube = unique_ptr<Cube>(new Cube());
	PointLight::I();
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void Scene::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	this->cube->update(deltaTime);
}

void Scene::renderScene()
{
	// this->plane->render();
	this->cube->render();
	this->gui->drawGui();
}
