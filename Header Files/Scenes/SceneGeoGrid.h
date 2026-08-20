#pragma once

#include "../Game Objects/IVisibleGameObject.h"
#include "../Gui/Gui.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

class SceneGeoGrid : public IScene
{
public:
	SceneGeoGrid(const shared_ptr<fvec3> clearColor);
	~SceneGeoGrid() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene(const float currentTime) override;

private:
	vector<shared_ptr<IVisibleGameObject>> gameObjects;

	unique_ptr<Gui> gui;
};
