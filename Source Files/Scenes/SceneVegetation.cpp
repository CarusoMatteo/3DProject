#include "../../Header Files/Scenes/SceneVegetation.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Camera/CameraPath.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneVegetation::SceneVegetation(const shared_ptr<fvec3> clearColor)
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{fvec3(-20, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmoother},
		{{fvec3(0, 2, 20), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.7f, easeInOutSmoother},
		{{fvec3(20, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmoother},
		{{fvec3(0, 2, -20), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 1.0f, easeInOutSmoother},
	});
	const CameraPath slideLeftToRight = CameraPath({
		{{fvec3(-5, 1, 7), fvec3(-5, 1, 0), fvec3(0, 1, 0)}, 1.0f, easeInOutSmoother},
		{{fvec3(5, 1, 7), fvec3(5, 1, 0), fvec3(0, 1, 0)}, 1.0f, easeInOutSmoother},
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
	LightManager::I({{.position = fvec3(0, 45, 10), .color = fvec3(1)}});

	Transform treeTransform1 = {fvec3(0, -5, 0), Rotation(0, fvec3(0, 1, 0)), fvec3(0.25f)};
	Transform treeTransform2 = {fvec3(8, -5, 2), Rotation(0, fvec3(0, 1, 0)), fvec3(0.25f)};
	auto treeTexture = TextureFactory::fromFile("./assets/models/tree/tree.png", false);

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("tree", treeTransform1, ShaderFactory::blinnPhong, treeTexture)),
		shared_ptr<IVisibleGameObject>(new CustomModel("tree", treeTransform2, ShaderFactory::blinnPhong, treeTexture)),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneVegetation::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneVegetation::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
