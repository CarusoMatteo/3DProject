#include "../../Header Files/Scenes/SceneGeoGrid.h"
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

SceneGeoGrid::SceneGeoGrid(const shared_ptr<fvec3> clearColor)
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{fvec3(-10, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmooth},
		{{fvec3(0, 2, 10), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.7f, easeLinear},
		{{fvec3(10, 2, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmoother},
		{{fvec3(0, 2, -10), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 1.0f, easeLinear},
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

	Camera::I(slideLeftToRight);
	LightManager::I({{.position = fvec3(45), .color = fvec3(1)}});

	Transform gridTransform = {fvec3(10, 1, 0), Rotation(90, fvec3(0, 1, 0)), fvec3(0.5, 1, 2)};

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("grid", gridTransform, ShaderFactory::blinnPhong, TextureFactory::none())),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));

	// this->scatterObjects();
}

void SceneGeoGrid::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}
}

void SceneGeoGrid::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}
	// ShaderFactory::geometry()->finishGeometryPass();
	// ShaderFactory::geometry()->lightingPass();

	this->gui->drawGui(this->gameObjects);
}
