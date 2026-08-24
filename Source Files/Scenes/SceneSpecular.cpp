#include "../../Header Files/Scenes/SceneSpecular.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Path/CameraPath.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneSpecular::SceneSpecular(const shared_ptr<fvec3> clearColor)
{
	Camera::I(CameraPath({
		{fvec3(-4, 0, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)},
		{fvec3(0, 0, 4), fvec3(0, 0, 0), fvec3(0, 1, 0)},
		{fvec3(4, 0, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)},
		{fvec3(0, 0, -4), fvec3(0, 0, 0), fvec3(0, 1, 0)},
	}));
	LightManager::I({{.position = fvec3(45), .color = fvec3(1)}});

	Transform sphereTransform = {fvec3(0), Rotation(), fvec3(1)};

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new Sphere(sphereTransform, ShaderFactory::reflection())),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneSpecular::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneSpecular::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
