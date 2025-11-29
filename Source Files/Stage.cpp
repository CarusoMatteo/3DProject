#include "../Header Files/Scenes/Scene.h"
#include "../Header Files/Stage.h"
#include "../Header Files/Window.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

Stage::Stage(const shared_ptr<fvec3> clearColor, const string windowTitle)
{
	this->clearColor = clearColor;
	Window::I();
	this->scene = unique_ptr<Scene>(new Scene());
}

void Stage::updateGameObjects(const float deltaTime)
{
	this->scene->updateGameObjects(deltaTime);
}

void Stage::renderScene() const
{
	this->scene->renderScene();
}

void Stage::drawClearColor() const
{
	glClearColor(this->clearColor.get()->x, this->clearColor.get()->y, this->clearColor.get()->z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
