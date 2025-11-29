#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

using namespace glm;
using namespace std;

class Window
{
public:
	Window(const string windowTitle);
	~Window() = default;

	bool shouldWindowClose();
	void swapBuffers();
	void pollEvents();
	ivec2 getWindowSize();

private:
	shared_ptr<GLFWwindow> window;

	void initializeWindow(string windowTitle, double monitorPercentageWidth, double monitorPercentageHeight);
	void initInputEvents();
	void initializeGui();
	void initOpenGL();
	static fmat4 createProjectionMatrix(int windowWidth, int windowHeight);
};
