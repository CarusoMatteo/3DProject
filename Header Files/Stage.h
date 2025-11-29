#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

using namespace glm;
using namespace std;

class IScene;
class Window;

class Stage
{
public:
	Stage(const fvec3 *clearColor, const string windowTitle);
	~Stage() = default;

	void updateGameObjects(float deltaTime);
	void renderScene() const;
	void drawClearColor() const;

	bool shouldWindowClose() const;
	void swapBuffers() const;
	void pollEvents() const;

private:
	shared_ptr<IScene> scene;
	const fvec3 *clearColor;
	shared_ptr<Window> window;
};
