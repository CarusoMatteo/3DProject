#include "../../Header Files/Scenes/SceneGrid.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Random.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneGrid::SceneGrid(const shared_ptr<fvec3> clearColor)
{
	Camera::I({
		{fvec3(-10, 0.5f, 0), fvec3(0, 0.5f, 0), fvec3(0, 1, 0), fvec3(0, 0.5f, 0) - fvec3(-10, 0.5f, 0)},
		{fvec3(-10, 0.5f, -15), fvec3(0, 0.5f, -15), fvec3(0, 1, 0), fvec3(0, 0.5f, -15) - fvec3(-10, 0.5f, -15)},
	});
	LightManager::I();

	Transform gridTransform = {fvec3(0, -1, 0), Rotation(), fvec3(5, 1, 5)};

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("grid", ShaderFactory::blinnPhong, TextureFactory::none())),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));

	// this->scatterObjects();
}

void SceneGrid::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneGrid::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}
	// ShaderFactory::geometry()->finishGeometryPass();
	// ShaderFactory::geometry()->lightingPass();

	this->gui->drawGui(this->gameObjects);
}
