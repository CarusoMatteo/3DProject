#pragma once

#include "../Game Objects/IVisibleGameObject.h"
#include "../Gui/Gui.h"
#include "../Path/Path.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

class SceneDisocclusion : public IScene
{
public:
	SceneDisocclusion(const shared_ptr<fvec3> clearColor);
	~SceneDisocclusion() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene(const float currentTime) override;

private:
	vector<shared_ptr<IVisibleGameObject>> gameObjects;
	Path cubePath;

	unique_ptr<Gui> gui;
};
