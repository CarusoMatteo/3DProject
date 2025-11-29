#include "../Header Files/Scenes/Scene.h"
#include "../Header Files/Stage.h"
#include "../Header Files/Window.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

Stage::Stage(const fvec3 *clearColor, const string windowTitle)
{
	this->clearColor = clearColor;
	this->window = make_shared<Window>(windowTitle);
	this->scene = make_shared<Scene>();
}

void Stage::updateGameObjects(float deltaTime)
{
}

void Stage::renderScene() const
{
}

void Stage::drawClearColor() const
{
}

bool Stage::shouldWindowClose() const
{
	return false;
}

void Stage::swapBuffers() const
{
}

void Stage::pollEvents() const
{
}
