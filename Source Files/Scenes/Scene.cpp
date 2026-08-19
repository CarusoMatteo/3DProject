#include "../../Header Files/Scenes/Scene.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Camera/CameraPath.h"
#include "../../Header Files/Camera/EasingFunctions.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Random.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

Scene::Scene(const shared_ptr<fvec3> clearColor)
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{fvec3(-10, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmooth},
		{{fvec3(0, 2, 10), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.7f, easeLinear},
		{{fvec3(10, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmoother},
		{{fvec3(0, 2, -10), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 1.0f, easeLinear},
	});
	const CameraPath slideLeftToRight = CameraPath({
		{{fvec3(-5, 1, 7), fvec3(-5, 1, 0), fvec3(0, 1, 0)}},
		{{fvec3(5, 1, 7), fvec3(5, 1, 0), fvec3(0, 1, 0)}},
	});
	const CameraPath slideTopToBottom = CameraPath({
		{{fvec3(0, -3, 7), fvec3(0, -3, 0), fvec3(0, 1, 0)}},
		{{fvec3(0, 3, 7), fvec3(0, 3, 0), fvec3(0, 1, 0)}},
	});
	const CameraPath slideBottomLeftToTopRight = CameraPath({
		{{fvec3(-5, -3, 10), fvec3(-5, -3, 0), fvec3(0, 1, 0)}},
		{{fvec3(5, 3, 10), fvec3(5, 3, 0), fvec3(0, 1, 0)}},
	});

	Camera::I(rotationAroundOrigin);
	LightManager::I();

	Transform planeTransform = {fvec3(0, -1, 0), Rotation(), fvec3(5, 1, 5)};
	auto backpackTexture = TextureFactory::fromFile("./assets/models/backpack/diffuse.jpg", true);

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new Plane(planeTransform, ShaderFactory::blinnPhong())),
		// shared_ptr<IVisibleGameObject>(new Cube(ShaderFactory::blinnPhong())),
		// shared_ptr<IVisibleGameObject>(new Sphere(ShaderFactory::blinnPhong())),
		shared_ptr<IVisibleGameObject>(new CustomModel("backpack", ShaderFactory::blinnPhong, backpackTexture)),
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
	// ShaderFactory::geometry()->finishGeometryPass();
	// ShaderFactory::geometry()->lightingPass();

	if (!InputEvents::shouldTakeScreenshotNextFrame(false))
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
