#pragma once

#include "../Game Objects/Camera.h"
#include "../Game Objects/Cube.h"
#include "../Gui/Gui.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

class Camera;
class Gui;

class Scene : public IScene
{
public:
	Scene(const shared_ptr<fvec3> clearColor);
	~Scene() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene() override;

private:
	unique_ptr<Camera> camera;
	unique_ptr<Cube> cube;
	unique_ptr<Gui> gui;
};