#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Scenes/Scene.h"
#include <glm/glm.hpp>
#include <memory>

Scene::Scene(const shared_ptr<fvec3> clearColor)
{
	this->camera = unique_ptr<Camera>(new Camera());
	this->cube = unique_ptr<Cube>(new Cube());
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void Scene::updateGameObjects(float deltaTime)
{
	this->camera->update(deltaTime);
}

void Scene::renderScene()
{
	this->camera->render();
	this->gui->drawGui();
}
