#include "../../Header Files/Scenes/SceneEdges.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Path/EasingFunctions.h"
#include "../../Header Files/Path/Path.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneEdges::SceneEdges(const shared_ptr<fvec3> clearColor)
	: cubePath({
		  PathState{{.position = fvec3(-3, 0, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(-3, -3, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(0, -3, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(3, -3, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(3, 0, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(3, 3, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(0, 3, 10)}, 10.0f, easeLinear},
		  PathState{{.position = fvec3(-3, 3, 10)}, 10.0f, easeLinear},
	  })
{
	const CameraPath stuck = CameraPath({
		{{.position = fvec3(0, 0, 20), .target = fvec3(0, 0, 0)}, 2.0f, easeLinear},
	});
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
		{{.position = fvec3(-5, 0, 10), .target = fvec3(-5, 0, 0)}, 2.0f, easeLinear},
		{{.position = fvec3(0, -3, 10), .target = fvec3(0, -3, 0)}, 2.0f, easeLinear},
		{{.position = fvec3(5, 0, 10), .target = fvec3(5, 0, 0)}, 2.0f, easeLinear},
		{{.position = fvec3(0, 3, 10), .target = fvec3(0, 3, 0)}, 2.0f, easeLinear},
	});

	Camera::I(stuck);
	LightManager::I({
		{.position = fvec3(0, 0, 50), .color = fvec3(1)},
	});

	Transform planeTransform = {.position = fvec3(0), .rotation = {.angle = 90, .axis = fvec3(1, 0, 0)}, .scale = fvec3(2)};
	auto planeTexture = TextureFactory::fromFile("./assets/models/white_plane/white_plane.png", true);

	Transform cubeTransform = {.position = fvec3(0), .rotation = {.angle = 90, .axis = fvec3(1, 0, 0)}, .scale = fvec3(1)};
	auto cubeTexture = TextureFactory::fromFile("./assets/models/cube/cube.png", true);

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("white_plane", planeTransform, ShaderFactory::blinnPhong, planeTexture)),
		shared_ptr<IVisibleGameObject>(new CustomModel("cube", cubeTransform, ShaderFactory::blinnPhong, cubeTexture)),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneEdges::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}

	// Only for the cube, update position along path
	shared_ptr<IVisibleGameObject> cube = this->gameObjects.at(2);
	cube->setTransform(this->cubePath.getNextTransform(deltaTime));
}

void SceneEdges::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
