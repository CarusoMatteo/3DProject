#include "../../Header Files/Scenes/SceneTest.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Path/CameraPath.h"
#include "../../Header Files/Path/EasingFunctions.h"
#include "../../Header Files/Random.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneTest::SceneTest(const shared_ptr<fvec3> clearColor)
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{fvec3(-10, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmooth},
		{{fvec3(0, 2, 10), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.7f, easeLinear},
		{{fvec3(10, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmoother},
		{{fvec3(0, 2, -10), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 1.0f, easeLinear},
	});
	const CameraPath slideLeftToRight = CameraPath({
		{{fvec3(-5, 1, 7), fvec3(-5, 1, 0), fvec3(0, 1, 0)}, 1.0f, easeLinear},
		{{fvec3(5, 1, 7), fvec3(5, 1, 0), fvec3(0, 1, 0)}, 1.0f, easeLinear},
	});
	const CameraPath slideTopToBottom = CameraPath({
		{{fvec3(0, -3, 7), fvec3(0, -3, 0), fvec3(0, 1, 0)}},
		{{fvec3(0, 3, 7), fvec3(0, 3, 0), fvec3(0, 1, 0)}},
	});
	const CameraPath slideBottomLeftToTopRight = CameraPath({
		{{fvec3(-5, -3, 10), fvec3(-5, -3, 0), fvec3(0, 1, 0)}},
		{{fvec3(5, 3, 10), fvec3(5, 3, 0), fvec3(0, 1, 0)}},
	});

	const CameraTransform defaultTransform = {.position = fvec3(-11, -0.5f, -25.7f), .target = fvec3(-10.5f, -0.6f, -24.8f)};
	Camera::I(defaultTransform.toCameraTransform());
	LightManager::I({{.position = fvec3(45), .color = fvec3(1)}});

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
}

void SceneTest::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneTest::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}

void SceneTest::scatterObjects()
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
