#include "../../Header Files/Scenes/SceneDisocclusion.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Path/Path.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneDisocclusion::SceneDisocclusion(const shared_ptr<fvec3> clearColor)
	: cubePath({
		  PathState{{.position = fvec3(-15, 0, 0)}, 3.0f, easeLinear},
		  PathState{{.position = fvec3(15, 0, 0)}, 3.0f, easeLinear},
	  })
{
	const CameraPath stuck = CameraPath({
		{{.position = fvec3(0, 0, 20), .target = fvec3(0)}, 2.0f, easeLinear},
	});

	Camera::I(stuck);
	LightManager::I({
		{.position = fvec3(0, 0, 50), .color = fvec3(1)},
	});

	Transform frontPillarTransform1 = {.position = fvec3(4.3f, 0, 4.3f), .rotation = {.angle = 90, .axis = fvec3(1, 0, 0)}, .scale = fvec3(1, 1, 10)};
	Transform frontPillarTransform2 = {.position = fvec3(-4.3f, 0, 4.3f), .rotation = {.angle = 90, .axis = fvec3(1, 0, 0)}, .scale = fvec3(1, 1, 10)};
	Transform backCubeTransform = {.position = fvec3(-6, 0, 0), .rotation = {.angle = 90, .axis = fvec3(1, 0, 0)}, .scale = fvec3(1)};

	auto blackTexture = TextureFactory::fromFile("./assets/models/cube/cube.png", true);
	auto whiteTexture = TextureFactory::fromFile("./assets/models/white_plane/white_plane.png", true);

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("cube", frontPillarTransform1, ShaderFactory::blinnPhong, blackTexture)),
		shared_ptr<IVisibleGameObject>(new CustomModel("cube", frontPillarTransform2, ShaderFactory::blinnPhong, blackTexture)),
		shared_ptr<IVisibleGameObject>(new CustomModel("cube", backCubeTransform, ShaderFactory::blinnPhong, whiteTexture)),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneDisocclusion::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}

	// Only for the movement cube, update position along path
	shared_ptr<IVisibleGameObject> cube = this->gameObjects.at(3);
	cube->setTransform(this->cubePath.getNextTransform(deltaTime));
}

void SceneDisocclusion::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
