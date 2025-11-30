#pragma once

#include "../Game Objects/Cube.h"
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
	// unique_ptr<Camera> camera; Use singleton access instead
	unique_ptr<Plane> plane;
	unique_ptr<Cube> cube;
	unique_ptr<PointLight> light;
	unique_ptr<Gui> gui;
};
