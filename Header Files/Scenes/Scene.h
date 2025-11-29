#pragma once

#include "IScene.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Cube;
class IGui;

/**
 * @brief Represents the menu scene of the game.
 * It only contains the background and the Gui.
 */
class Scene : public IScene
{
public:
	Scene() = default;
	~Scene() = default;

	void updateGameObjects(float deltaTime) override;
	void renderScene() override;

private:
	Cube *cube;
	IGui *gui;
	ivec2 windowSize;

	void renderGui();
};