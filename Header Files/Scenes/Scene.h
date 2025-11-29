#pragma once

#include "IScene.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Camera;

class Scene : public IScene
{
public:
	Scene();
	~Scene() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene() override;

private:
	unique_ptr<Camera> camera;
	// Cube *cube;
	// IGui *gui;
	// ivec2 windowSize;

	void renderGui();
};