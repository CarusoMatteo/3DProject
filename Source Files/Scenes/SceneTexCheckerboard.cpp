#include "../../Header Files/Scenes/SceneTexCheckerboard.h"
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

SceneTexCheckerboard::SceneTexCheckerboard(const shared_ptr<fvec3> clearColor)
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{.position = fvec3(-10, 2, 0), .target = fvec3(0, 0, 0)}, 0.5f, easeInOutSmooth},
		{{.position = fvec3(0, 2, 10), .target = fvec3(0, 0, 0)}, 0.7f, easeLinear},
		{{.position = fvec3(10, 2, 0), .target = fvec3(0, 0, 0)}, 0.5f, easeInOutSmoother},
		{{.position = fvec3(0, 2, -10), .target = fvec3(0, 0, 0)}, 1.0f, easeLinear},
	});
	const CameraPath slideLeftToRight = CameraPath({
		{{.position = fvec3(-5, 0, 5), .target = fvec3(-5, 0, 0)}, 1.0f, easeLinear},
		{{.position = fvec3(5, 0, 5), .target = fvec3(5, 0, 0)}, 1.0f, easeLinear},
	});
	const CameraPath slideTopToBottom = CameraPath({
		{{.position = fvec3(0, -3, 7), .target = fvec3(0, -3, 0)}},
		{{.position = fvec3(0, 3, 7), .target = fvec3(0, 3, 0)}},
	});
	const CameraPath slideBottomLeftToTopRight = CameraPath({
		{{.position = fvec3(-5, -3, 10), .target = fvec3(-5, -3, 0)}},
		{{.position = fvec3(5, 3, 10), .target = fvec3(5, 3, 0)}},
	});
	const CameraPath slideRombus = CameraPath({
		{{.position = fvec3(-2, 0, 10), .target = fvec3(-2, 0, 0)}},
		{{.position = fvec3(0, -5, 10), .target = fvec3(0, -5, 0)}},
		{{.position = fvec3(2, 0, 10), .target = fvec3(2, 0, 0)}},
		{{.position = fvec3(0, 5, 10), .target = fvec3(0, 5, 0)}},
	});

	Camera::I(slideRombus);
	LightManager::I({{.position = fvec3(0, 0, 50), .color = fvec3(1)}});

	Transform planeTransform = {.position = fvec3(0), .rotation = {.angle = 90, .axis = fvec3(1, 0, 0)}, .scale = fvec3(1)};
	auto planeTexture = TextureFactory::fromFile("./assets/models/checkerboard/checkerboard.png", true);

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("checkerboard", planeTransform, ShaderFactory::blinnPhong, planeTexture)),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneTexCheckerboard::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneTexCheckerboard::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
