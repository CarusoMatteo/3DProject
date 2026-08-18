#include "../Header Files/Stage.h"
#include "../Header Files/InputEvents.h"
#include "../Header Files/Scenes/IScene.h"
#include "../Header Files/Scenes/Scene.h"
#include "../Header Files/Scenes/SceneSpecular.h"
#include "../Header Files/Texture/TextureLoader.h"
#include "../Header Files/Window.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

Stage::Stage(const shared_ptr<fvec3> clearColor)
{
	this->clearColor = clearColor;
	Window::I();

	setupFBO();

	this->scene = unique_ptr<IScene>(new Scene(clearColor));
}

void Stage::updateGameObjects(const float deltaTime)
{
	this->scene->updateGameObjects(deltaTime);
}

void Stage::renderScene(const float currentTime) const
{
	const ivec2 size = Window::I()->getSize();
	// 1. Render the scene in the FBO (with MRT enabled)
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->scene->renderScene(currentTime);

	// 2. Copy (blit) the main buffer on the screen
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default framebuffer
	glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// 3. Read the pixels of the texture to save
	saveBuffers(currentTime, size);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

	// Depth (necessary because of the depth test)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLenum drawBuffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Warning: Incomplete FBO!" << endl;
		throw runtime_error("Error: Incomplete FBO.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Stage::saveBuffers(const float currentTime, const ivec2 size) const
{
	if (InputEvents::shouldTakeScreenshotNextFrame())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		string filename;
		vector<float> pixelsFloat(size.x * size.y * 4);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		filename = ("img/" + to_string(currentTime) + "_main.bmp");
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		saveTexture(size, pixelsFloat, filename);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
		filename = ("img/" + to_string(currentTime) + "_normal.bmp");
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		saveTexture(size, pixelsFloat, filename);

		glReadBuffer(GL_COLOR_ATTACHMENT3);
		filename = ("img/" + to_string(currentTime) + "_depth.bmp");
		glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_FLOAT, pixelsFloat.data());
		saveTexture(size, pixelsFloat, filename);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
