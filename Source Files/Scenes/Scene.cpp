#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Random.h"
#include "../../Header Files/Scenes/Scene.h"
#include <glm/glm.hpp>
#include <memory>

Scene::Scene(const shared_ptr<fvec3> clearColor)
{
	Camera::I();
	this->plane = unique_ptr<Plane>(new Plane());
	this->cubes = {
		shared_ptr<Cube>(new Cube()),
		shared_ptr<Cube>(new Cube()),
		shared_ptr<Cube>(new Cube())};
	this->spheres = {
		shared_ptr<Sphere>(new Sphere()),
		shared_ptr<Sphere>(new Sphere()),
		shared_ptr<Sphere>(new Sphere())};
	PointLight::I();
	this->gui = unique_ptr<Gui>(new Gui(clearColor));

	this->scatterObjects();
}

void Scene::updateGameObjects(float deltaTime)
{
	PointLight::I()->update(deltaTime);
	Camera::I()->update(deltaTime);
	this->plane->update(deltaTime);

	for (auto &cube : this->cubes)
	{
		cube->update(deltaTime);
	}
	for (auto &sphere : this->spheres)
	{
		sphere->update(deltaTime);
	}
}

void Scene::renderScene()
{
	this->plane->render();

	for (auto &cube : this->cubes)
	{
		cube->render();
	}
	for (auto &sphere : this->spheres)
	{
		sphere->render();
	}

	this->gui->drawGui();
}

void Scene::scatterObjects()
{
	const float min = -5.0f;
	const float max = 5.0f;

	for (auto &&cube : this->cubes)
	{
		cube->setTransform(Transform{
			fvec3(Random::getRandomFloat(min, max), 0, Random::getRandomFloat(min, max)),
			Rotation(),
			fvec3(1)});
	}
	for (auto &&sphere : this->spheres)
	{
		sphere->setTransform(Transform{
			fvec3(Random::getRandomFloat(min, max), 0, Random::getRandomFloat(min, max)),
			Rotation(),
			fvec3(1)});
	}
}
