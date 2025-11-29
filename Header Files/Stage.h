#pragma once

#include "Scenes/Scene.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class Scene;
class Window;

class Stage
{
public:
	Stage(const shared_ptr<fvec3> clearColor, const string windowTitle);
	~Stage() = default;

	void updateGameObjects(const float deltaTime);
	void renderScene() const;
	void drawClearColor() const;

	bool shouldWindowClose() const;
	void swapBuffers() const;
	void pollEvents() const;

private:
	unique_ptr<Scene> scene;
	shared_ptr<fvec3> clearColor;
};
