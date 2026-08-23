#include "../Header Files/Stage.h"
#include "../Header Files/InputEvents.h"
#include "../Header Files/Scenes/IScene.h"
#include "../Header Files/Scenes/SceneEdges.h"
#include "../Header Files/Scenes/SceneVegetation.h"
#include "../Header Files/Scenes/SceneGeoGrid.h"
#include "../Header Files/Scenes/SceneGeoSpiral.h"
#include "../Header Files/Scenes/SceneGeoThinTriangles.h"
#include "../Header Files/Scenes/SceneTest.h"
#include "../Header Files/Scenes/SceneTexBricks.h"
#include "../Header Files/Scenes/SceneTexCheckerboard.h"
#include "../Header Files/Scenes/SceneTexRepeatedPatterns.h"
#include "../Header Files/Texture/TextureLoader.h"
#include "../Header Files/Window.h"
#include <GLFW/glfw3.h>
#include <format>
#include <future>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

static string formatDuration(const double seconds)
{
	const long long wholeSeconds = static_cast<long long>(seconds);
	const long long totalSeconds = wholeSeconds > 0 ? wholeSeconds : 0;
	const long long hours = totalSeconds / 3600;
	const long long minutes = (totalSeconds % 3600) / 60;
	const long long remainingSeconds = totalSeconds % 60;

	ostringstream formatted;
	if (hours > 0)
		formatted << hours << ':';
	formatted << setfill('0') << setw(2) << minutes << ':'
			  << setw(2) << remainingSeconds;
	return formatted.str();
}

Stage::Stage(const shared_ptr<fvec3> clearColor, const bool fixedWindowSize)
{
	this->clearColor = clearColor;
	Window::I(fixedWindowSize);

	setupFBO();
	setupFBO4k();

	this->scene = unique_ptr<IScene>(new SceneVegetation(clearColor));
}

void Stage::updateGameObjects(const float deltaTime)
{
	// Ignore deltaTime if we're currently saving screenshots, to avoid skipping frames in the scene update.
	if (this->screenshotIndex > 0)
		this->scene->updateGameObjects(1 / 60.0f);
	else
		this->scene->updateGameObjects(deltaTime);
}

void Stage::renderScene(const float currentTime)
{
	const ivec2 size = Window::I()->getSize();
	// 1.1 Render the scene in the FBO (with MRT enabled) at native resolution
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->scene->renderScene(currentTime);

	// 1.2 Copy (blit) the main buffer on the screen
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default framebuffer
	glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// If we need to take a screenshot, also render the scene in the FBO_4K (with MRT enabled)
	if (InputEvents::shouldTakeScreenshotNextFrame(false))
	{
		// 2.1. Render the scene in the FBO_4K (with MRT enabled)
		glBindFramebuffer(GL_FRAMEBUFFER, fbo4k);
		// temporarily set the viewport to 4K resolution
		glViewport(0, 0, 3840, 2160);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->scene->renderScene(currentTime);

		// Reset the viewport to the window size
		glViewport(0, 0, size.x, size.y);
	}

	// 3. Read the pixels of the texture to save
	addBuffersToSaveQueue(currentTime, size);
}

void Stage::drawClearColor() const
{
	glClearColor(this->clearColor.get()->r, this->clearColor.get()->g, this->clearColor.get()->b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Stage::shouldWindowClose() const
{
	return Window::I()->shouldClose();
}

void Stage::swapBuffers() const
{
	Window::I()->swapBuffers();
}

void Stage::pollEvents() const
{
	Window::I()->pollEvents();
}

void Stage::setupFBO()
{
	const ivec2 size = Window::I()->getSize();
	unsigned int texColor = -1;
	unsigned int texFileColor = -1;
	unsigned int texNormal = -1;
	unsigned int texDepth = -1;
	unsigned int texMotionVectors = -1;
	unsigned int rboDepth = -1;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Texture 0: The one that will be shown on screen
	glGenTextures(1, &texColor);
	glBindTexture(GL_TEXTURE_2D, texColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

	// Texture 1: The one that will be saved to file.
	glGenTextures(1, &texFileColor);
	glBindTexture(GL_TEXTURE_2D, texFileColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texFileColor, 0);

	// Texture 2: Normal texture that will be saved to file.
	glGenTextures(1, &texNormal);
	glBindTexture(GL_TEXTURE_2D, texNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, texNormal, 0);

	// Texture 3: Depth texture that will be saved to file.
	glGenTextures(1, &texDepth);
	glBindTexture(GL_TEXTURE_2D, texDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, texDepth, 0);

	// Texture 4: Motion vectors texture that will be saved to file.
	glGenTextures(1, &texMotionVectors);
	glBindTexture(GL_TEXTURE_2D, texMotionVectors);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, texMotionVectors, 0);

	// Depth (necessary because of the depth test)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLenum drawBuffers[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	glDrawBuffers(5, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Warning: Incomplete FBO!" << endl;
		throw runtime_error("Error: Incomplete FBO.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Stage::setupFBO4k()
{
	const ivec2 size = {3840, 2160};
	unsigned int texFileColor = -1;
	unsigned int rboDepth = -1;

	glGenFramebuffers(1, &fbo4k);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo4k);

	// Texture 1: The one that will be saved to file.
	glGenTextures(1, &texFileColor);
	glBindTexture(GL_TEXTURE_2D, texFileColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texFileColor, 0);

	// Depth (necessary because of the depth test)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT1};
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Warning: Incomplete FBO!" << endl;
		throw runtime_error("Error: Incomplete FBO.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Stage::addBuffersToSaveQueue(const float currentTime, const ivec2 size)
{
	// Consume input only when we've saved the correct amount of screenshots in a row.
	const bool shouldConsumeInput = this->screenshotIndex >= this->screenshotsInARowCount - 1;
	const bool shouldTakeScreenshot = InputEvents::shouldTakeScreenshotNextFrame(shouldConsumeInput);

	if (shouldTakeScreenshot)
	{
		// How many buffers we have per screenshot data (main, normal, depth, motion vectors, main4k)
		const unsigned int screenshotsPerData = 5;
		// Total number of screenshots to save
		const unsigned int total = this->screenshotsInARowCount * screenshotsPerData;
		// Estimated time to save one screenshot in seconds
		const double estimateOneScreenshot = 6.0;
		// Estimated time to save all screenshots in seconds without parallelization
		const double estimatedTimeIndividual = estimateOneScreenshot * total;
		// Estimated time to save all screenshots in seconds with parallelization
		const double estimatedTimeAll = estimateOneScreenshot * this->screenshotsInARowCount;
		// Whether to save the screenshots as PNG files or not
		const bool saveToPng = true;

		auto saveAsync = [&](const ScreenshotTuple &buffer)
		{
			return async(
				launch::async,
				[&buffer, saveToPng]()
				{
					const double currentTime = glfwGetTime();
					saveTexture(buffer.size, buffer.pixelsFloat, buffer.filename, saveToPng, false);
					return glfwGetTime() - currentTime;
				});
		};

		// First screenshot of the series
		if (this->screenshotIndex == 0)
		{
			cout << "Saving screenshots to disk...\t\t\t\t\t\tEstimated time left: " << formatDuration(estimatedTimeAll) << endl;
			this->timeSum = 0;
			this->startTime = glfwGetTime();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		string filename;
		vector<float> pixelsFloat(size.x * size.y * 4);
		ScreenshotData data;

		// 0 based
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		filename = ("img/main_" + format("{:03}", this->screenshotIndex));
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		data.mainBuffer = {filename, size, pixelsFloat};
		future<double> mainSave = saveAsync(data.mainBuffer);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
		filename = ("img/normal_" + format("{:03}", this->screenshotIndex));
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		data.normalBuffer = {filename, size, pixelsFloat};
		future<double> normalSave = saveAsync(data.normalBuffer);

		glReadBuffer(GL_COLOR_ATTACHMENT3);
		filename = ("img/depth_" + format("{:03}", this->screenshotIndex));
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		data.depthBuffer = {filename, size, pixelsFloat};
		future<double> depthSave = saveAsync(data.depthBuffer);

		glReadBuffer(GL_COLOR_ATTACHMENT4);
		filename = ("img/motion_" + format("{:03}", this->screenshotIndex));
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		data.motionVectorsBuffer = {filename, size, pixelsFloat};
		future<double> motionVectorsSave = saveAsync(data.motionVectorsBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo4k);

		pixelsFloat.resize(3840 * 2160 * 4);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		filename = ("img/main4k_" + format("{:03}", this->screenshotIndex));
		glReadPixels(0, 0, 3840, 2160, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		data.mainBuffer4k = {filename, {3840, 2160}, pixelsFloat};
		future<double> mainBuffer4kSave = saveAsync(data.mainBuffer4k);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Wait for async end here
		const double mainSaveTime = mainSave.get();
		timeSum += mainSaveTime;
		cout << "Saved screenshots  "
			 << this->screenshotIndex * 5 + 1 << " / " << total << "  ("
			 << (this->screenshotIndex * 5.0 + 1) / total * 100 << "%) to disk."
			 << "\tTook " << mainSaveTime << " seconds." << endl;

		const double normalSaveTime = normalSave.get();
		timeSum += normalSaveTime;
		cout << "Saved screenshots  "
			 << this->screenshotIndex * 5 + 2 << " / " << total << "  ("
			 << (this->screenshotIndex * 5.0 + 2) / total * 100 << "%) to disk."
			 << "\tTook " << normalSaveTime << " seconds." << endl;

		const double depthSaveTime = depthSave.get();
		timeSum += depthSaveTime;
		cout << "Saved screenshots  "
			 << this->screenshotIndex * 5 + 3 << " / " << total << "  ("
			 << (this->screenshotIndex * 5.0 + 3) / total * 100 << "%) to disk."
			 << "\tTook " << depthSaveTime << " seconds." << endl;

		const double motionVectorsSaveTime = motionVectorsSave.get();
		timeSum += motionVectorsSaveTime;
		cout << "Saved screenshots  "
			 << this->screenshotIndex * 5 + 4 << " / " << total << "  ("
			 << (this->screenshotIndex * 5.0 + 4) / total * 100 << "%) to disk."
			 << "\tTook " << motionVectorsSaveTime << " seconds." << endl;

		const double mainBuffer4kSaveTime = mainBuffer4kSave.get();
		timeSum += mainBuffer4kSaveTime;
		cout << "Saved screenshots  "
			 << this->screenshotIndex * 5 + 5 << " / " << total << "  ("
			 << (this->screenshotIndex * 5.0 + 5) / total * 100 << "%) to disk."
			 << "\tTook " << mainBuffer4kSaveTime << " seconds."
			 << "\tEstimated time left: " << formatDuration(estimatedTimeAll - estimateOneScreenshot * this->screenshotIndex / screenshotsPerData) << endl;

		this->screenshotIndex += 1;

		if (this->screenshotIndex >= this->screenshotsInARowCount)
		{
			cout << "Saved all screenshots to disk.\t\t\tTook " << formatDuration(glfwGetTime() - startTime) << " compared to the estimate: " << formatDuration(estimatedTimeAll) << endl
				 << "Average time per screenshot: " << timeSum / total << " seconds." << endl;
			this->screenshotIndex = 0;
		}
	}
}
