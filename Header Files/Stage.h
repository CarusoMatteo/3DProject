#pragma once

#include "Scenes/IScene.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

class Window;

class Stage
{
public:
	Stage(const shared_ptr<fvec3> clearColor);
	~Stage() = default;

	void updateGameObjects(const float deltaTime);
	void renderScene(const float currentTime) const;
	void drawClearColor() const;

	bool shouldWindowClose() const;
	void swapBuffers() const;
	void pollEvents() const;

private:
	unique_ptr<IScene> scene;
	shared_ptr<fvec3> clearColor;

	unsigned int fbo = -1;

	void setupFBO();
	void saveBuffers(const float currentTime, const ivec2 size) const;
};
