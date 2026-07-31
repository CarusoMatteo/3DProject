#include "../../Header Files/Scenes/Scene.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Random.h"
#include <glm/glm.hpp>
#include <memory>

Scene::Scene(const shared_ptr<fvec3> clearColor)
{
	Camera::I();
	this->gameObjects = {
		// shared_ptr<Skybox>(new Skybox()),
		shared_ptr<Plane>(new Plane()),
		// shared_ptr<Cube>(new Cube()),
		// shared_ptr<Cube>(new Cube()),
		// shared_ptr<Cube>(new Cube()),
		shared_ptr<Sphere>(new Sphere()),
		// shared_ptr<Sphere>(new Sphere()),
		// shared_ptr<Sphere>(new Sphere()),
	};
	PointLight::I();
	this->gui = unique_ptr<Gui>(new Gui(clearColor));

	// this->scatterObjects();
}

void Scene::updateGameObjects(float deltaTime)
{
	PointLight::I()->update(deltaTime);
	Camera::I()->update(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void Scene::renderScene()
{
	for (auto &&object : this->gameObjects)
	{
		object->render();
	}

	this->gui->drawGui(this->gameObjects);
}

void Scene::scatterObjects()
{
	const float min = -10.0f;
	const float max = 10.0f;

	bool isFirst = true;

	for (auto &&object : this->gameObjects)
	{
		if (isFirst)
		{
			// Skip the plane
			isFirst = false;
			continue;
		}

		object->setTransform(Transform{
			fvec3(Random::getRandomFloat(min, max), 0, Random::getRandomFloat(min, max)),
			Rotation(),
			fvec3(1)});
	}
}
