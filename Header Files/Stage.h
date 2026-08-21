#pragma once

#include "Scenes/IScene.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

class Window;

class Stage
{
public:
	Stage(const shared_ptr<fvec3> clearColor, const bool fixedWindowSize);
	~Stage() = default;

	void updateGameObjects(const float deltaTime);
	void renderScene(const float currentTime);
	void drawClearColor() const;

	bool shouldWindowClose() const;
	void swapBuffers() const;
	void pollEvents() const;

private:
	const unsigned int screenshotsInARowCount = 20;
	unsigned int screenshotIndex = 0;
	double timeSum;
	double startTime;

	unique_ptr<IScene> scene;
	shared_ptr<fvec3> clearColor;

	unsigned int fbo = -1;
	unsigned int fbo4k = -1;

	void setupFBO();
	void setupFBO4k();
	void addBuffersToSaveQueue(const float currentTime, const ivec2 size);
};
