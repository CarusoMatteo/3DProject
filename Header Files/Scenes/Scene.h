#pragma once

#include "../Game Objects/SimpleObject.h"
#include "../Game Objects/Plane.h"
#include "../Game Objects/PointLight.h"
#include "../Gui/Gui.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

class Scene : public IScene
{
public:
	Scene(const shared_ptr<fvec3> clearColor);
	~Scene() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene() override;

private:
	// Do not save. Use singleton access instead
	// unique_ptr<Camera> camera;

	unique_ptr<Plane> plane;
	unique_ptr<SimpleObject> cube;
	unique_ptr<PointLight> light;
	unique_ptr<Gui> gui;
};
