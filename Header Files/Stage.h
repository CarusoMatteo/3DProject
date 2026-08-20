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
	Stage(const shared_ptr<fvec3> clearColor);
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
	};

	const unsigned int screenshotsInARowCount = 100;
	vector<ScreenshotData> screenshotQueue;

	unique_ptr<IScene> scene;
	shared_ptr<fvec3> clearColor;

	unsigned int fbo = -1;

	void setupFBO();
	void addBuffersToSaveQueue(const float currentTime, const ivec2 size);
	void saveBuffers() const;
};
