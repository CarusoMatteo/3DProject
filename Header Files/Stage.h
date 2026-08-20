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
	struct ScreenshotTuple
	{
		string filename;
		ivec2 size;
		vector<float> pixelsFloat;
	};

	struct ScreenshotData
	{
		ScreenshotTuple mainBuffer;
		ScreenshotTuple normalBuffer;
		ScreenshotTuple depthBuffer;
		ScreenshotTuple motionVectorsBuffer;
		ScreenshotTuple mainBuffer4k;
	};

	const unsigned int screenshotsInARowCount = 1;
	vector<ScreenshotData> screenshotQueue;

	unique_ptr<IScene> scene;
	shared_ptr<fvec3> clearColor;

	unsigned int fbo = -1;
	unsigned int fbo4k = -1;

	void setupFBO();
	void setupFBO4k();
	void addBuffersToSaveQueue(const float currentTime, const ivec2 size);
	void saveBuffers() const;
};
