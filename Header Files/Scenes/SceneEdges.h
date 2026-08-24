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

class SceneEdges : public IScene
{
public:
	SceneEdges(const shared_ptr<fvec3> clearColor);
	~SceneEdges() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene(const float currentTime) override;

private:
	vector<shared_ptr<IVisibleGameObject>> gameObjects;
	Path cubePath;

	unique_ptr<Gui> gui;
};
