#pragma once

#include "../../Header Files/Path/Path.h"
#include "../Game Objects/IVisibleGameObject.h"
#include "../Gui/Gui.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

class SceneValidation : public IScene
{
public:
	SceneValidation(const shared_ptr<fvec3> clearColor);
	~SceneValidation() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene(const float currentTime) override;

private:
	vector<shared_ptr<IVisibleGameObject>> gameObjects;

	Path spherePath;
	Path treePath;

	unique_ptr<Gui> gui;
};
