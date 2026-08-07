#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>

using namespace glm;
using namespace std;

class Window
{
public:
	// Singleton access.
	static Window *I();
	~Window();

	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	ivec2 getSize();
	fvec2 getWindowCenterCoordinates();

	void centerCursor();
	void disableCursor();
	void showCursor();

private:
	static optional<unique_ptr<Window>> instance;

	const string windowTitle = "3D Project";
	const fvec2 monitorPercentageSize = fvec2(0.8f);

	Window();

	// Can't be smart pointer.
	GLFWwindow *window;

	void initializeWindow();
	void initInputEvents();
	void initializeGui();
	void initOpenGL();
};
