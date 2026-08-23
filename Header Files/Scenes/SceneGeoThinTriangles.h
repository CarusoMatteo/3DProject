#pragma once

#include "../Game Objects/IVisibleGameObject.h"
#include "../Gui/Gui.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

class SceneGeoThinTriangles : public IScene
{
public:
	SceneGeoThinTriangles(const shared_ptr<fvec3> clearColor);
	~SceneGeoThinTriangles() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene(const float currentTime) override;

private:
	vector<shared_ptr<IVisibleGameObject>> gameObjects;

	unique_ptr<Gui> gui;
};
