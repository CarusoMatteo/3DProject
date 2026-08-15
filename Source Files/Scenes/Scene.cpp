#include "../../Header Files/Scenes/Scene.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Random.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include <glm/glm.hpp>
#include <memory>

Scene::Scene(const shared_ptr<fvec3> clearColor)
{
	Camera::I();
	LightManager::I();

	Transform planeTransform = {fvec3(0, -1, 0), Rotation(), fvec3(5, 1, 5)};
	// Transform cubeTransform = {fvec3(-1, 0, 0), Rotation(), fvec3(1)};
	// Transform sphereTransform = {fvec3(1, 0, 0), Rotation(), fvec3(1)};

	// auto backpackTexture = TextureFactory::fromFile("./assets/models/backpack/diffuse.jpg", true);

	this->gameObjects = {
		// shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new Plane(planeTransform, ShaderFactory::geometry()))
		// shared_ptr<IVisibleGameObject>(new CustomModel("backpack", ShaderFactory::geometry, backpackTexture))
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));

	// this->scatterObjects();
}

void Scene::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void Scene::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}
	ShaderFactory::geometry()->finishGeometryPass();
	ShaderFactory::geometry()->lightingPass();

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
