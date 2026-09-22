#include "../../Header Files/Scenes/SceneValidation.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Game Objects/Cube.h"
#include "../../Header Files/Game Objects/CustomModel.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/Plane.h"
#include "../../Header Files/Game Objects/Skybox.h"
#include "../../Header Files/Game Objects/Sphere.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Lights/LightValue.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Path/CameraPath.h"
#include "../../Header Files/Random.h"
#include "../../Header Files/Renderers/ShaderFactory.h"
#include "../../Header Files/Texture/TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>

SceneValidation::SceneValidation(const shared_ptr<fvec3> clearColor)
	: spherePath({
		  PathState{{fvec3(4.5f, 1, -5.5f), Rotation(0, fvec3(0, 1, 0))}, 1.0f, easeInOutSmoother},
		  PathState{{fvec3(4.5f, 1, 5.5f), Rotation(0, fvec3(0, 1, 0))}, 1.0f, easeInOutSmoother},
	  }),
	  treePath({
		  PathState{{fvec3(5, 0, -10), Rotation(90, fvec3(0, 1, 0)), fvec3(0.2f)}, 1.0f, easeInOutSmooth},
		  PathState{{fvec3(5, 0, -10), Rotation(180, fvec3(0, 1, 0)), fvec3(0.2f)}, 1.0f, easeInOutSmooth},
	  })
{
	const CameraPath rotationAroundOrigin = CameraPath({
		{{fvec3(-20, 4, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeLinear},
		{{fvec3(0, 4, 20), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.7f, easeInOutSmooth},
		{{fvec3(20, 4, 0), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 0.5f, easeInOutSmoother},
		{{fvec3(0, 4, -20), fvec3(0, 0, 0), fvec3(0, 1, 0)}, 1.0f, easeInOutSmooth},
	});

	Camera::I(rotationAroundOrigin);
	LightManager::I({{.position = fvec3(0, 45, 10), .color = fvec3(1)}});

	Transform planeTransform = {fvec3(0), Rotation(0, fvec3(0, 1, 0)), fvec3(1)};
	Transform cubeTransform = {fvec3(-9, 1, -4), Rotation(0, fvec3(0, 1, 0)), fvec3(1)};
	Transform sphereTransform = {fvec3(4.5f, 1, -5.5f), Rotation(0, fvec3(0, 1, 0)), fvec3(1)};
	Transform tree1Transform = {fvec3(-5, 0, 2.1f), Rotation(0, fvec3(0, 1, 0)), fvec3(0.2f)};
	Transform tree2Transform = {fvec3(5, 0, -10), Rotation(90, fvec3(0, 1, 0)), fvec3(0.2f)};
	Transform carTransform = {fvec3(4.2f, 0, 14.9f), Rotation(45, fvec3(0, 1, 0)), fvec3(1)};
	auto planeTexture = TextureFactory::fromFile("./assets/models/plane/ground.png", false);
	auto treeTexture = TextureFactory::fromFile("./assets/models/tree/tree.png", false);

	this->gameObjects = {
		shared_ptr<IVisibleGameObject>(new Skybox()),
		shared_ptr<IVisibleGameObject>(new CustomModel("plane", planeTransform, ShaderFactory::blinnPhong, planeTexture)),
		shared_ptr<IVisibleGameObject>(new Cube(cubeTransform, ShaderFactory::reflection())),
		shared_ptr<IVisibleGameObject>(new Sphere(sphereTransform, ShaderFactory::blinnPhong())),
		shared_ptr<IVisibleGameObject>(new CustomModel("tree", tree1Transform, ShaderFactory::blinnPhong, treeTexture)),
		shared_ptr<IVisibleGameObject>(new CustomModel("tree", tree2Transform, ShaderFactory::blinnPhong, treeTexture)),
		shared_ptr<IVisibleGameObject>(new CustomModel("car2", carTransform, ShaderFactory::blinnPhong, TextureFactory::none())),

		// shared_ptr<IVisibleGameObject>(new CustomModel("tree", treeTransform1, ShaderFactory::blinnPhong, treeTexture)),
		// shared_ptr<IVisibleGameObject>(new CustomModel("tree", treeTransform2, ShaderFactory::blinnPhong, treeTexture)),
	};
	this->gui = unique_ptr<Gui>(new Gui(clearColor));
}

void SceneValidation::updateGameObjects(float deltaTime)
{
	Camera::I()->update(deltaTime);
	LightManager::I()->updateLights(deltaTime);

	for (auto &&object : this->gameObjects)
	{
		object->update(deltaTime);
	}

	// Only for the movement sphere, update position along path
	shared_ptr<IVisibleGameObject> sphere = this->gameObjects.at(3);
	sphere->setTransform(this->spherePath.getNextTransform(deltaTime));
	// Only for the movement tree, update position along path
	shared_ptr<IVisibleGameObject> tree = this->gameObjects.at(5);
	tree->setTransform(this->treePath.getNextTransform(deltaTime));
}

void SceneValidation::renderScene(float currentTime)
{
	for (auto &&object : this->gameObjects)
	{
		object->render(currentTime);
	}

	this->gui->drawGui(this->gameObjects);
}
