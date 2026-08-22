#include "../../Header Files/Scenes/SceneGeoSpiral.h"
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

SceneGeoSpiral::SceneGeoSpiral(const shared_ptr<fvec3> clearColor)
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{fvec3(-2, 0, 0), fvec3(0, 0, 0)}, 0.7f, easeLinear},
		{{fvec3(0, 0, 2), fvec3(0, 0, 0)}, 0.7f, easeLinear},
		{{fvec3(2, 0, 0), fvec3(0, 0, 0)}, 0.7f, easeLinear},
		{{fvec3(0, 0, -2), fvec3(0, 0, 0)}, 0.7f, easeLinear},
	});
	const CameraPath slideUpToDown = CameraPath({
		{{fvec3(0, 1, 2), fvec3(0, 1, 0)}, 1.0f, easeInOutSmoother},
		{{fvec3(0, -1, 2), fvec3(0, -1, 0)}, 1.0f, easeInOutSmoother},
	});
	const CameraPath slideLeftToRight = CameraPath({
		{{fvec3(-5, 1, 7), fvec3(-5, 1, 0)}, 1.0f, easeInOutSmoother},
		{{fvec3(5, 1, 7), fvec3(5, 1, 0)}, 1.0f, easeInOutSmoother},
	});
	const CameraPath slideTopToBottom = CameraPath({
		{{fvec3(0, -3, 7), fvec3(0, -3, 0)}, 1.0f, easeInOutSmoother},
		{{fvec3(0, 3, 7), fvec3(0, 3, 0)}, 1.0f, easeInOutSmoother},
	});
	const CameraPath slideBottomLeftToTopRight = CameraPath({
		{{fvec3(-5, -3, 10), fvec3(-5, -3, 0)}, 1.0f, easeInOutSmoother},
		{{fvec3(5, 3, 10), fvec3(5, 3, 0)}, 1.0f, easeInOutSmoother},
	});

	Camera::I(slideUpToDown);
	LightManager::I({{.position = fvec3(0, 45, 10), .color = fvec3(1)}});

	Transform screwTransform = {.position = fvec3(0), .rotation = {.angle = 90, .axis = fvec3(0, 1, 0)}};

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("screw", screwTransform, ShaderFactory::blinnPhong, TextureFactory::none())),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneGeoSpiral::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneGeoSpiral::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
