#pragma once

#include "ButtonStates.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

/**
 * Static class to handle input events from GLFW with input callbacks.
 */
class InputEvents
{
public:
	static void keyCallback(const GLFWwindow *window, const int key, const int scancode, const int action, const int mods);
	static void cursorPositionCallback(const GLFWwindow *window, const double xpos, const double ypos);
	static void framebufferSizeCallback(const GLFWwindow *window, const int width, const int height);
	// Add more callbacks as needed here

	static ButtonStates *getButtonStates();
	static dvec2 *getCursorPosition();
	static ivec2 getCurrentWindowSize();
	static void setCurrentWindowSize(const ivec2 newSize);

private:
	static ButtonStates buttonStates;
	static dvec2 cursorPosition;
	static ivec2 currentWindowSize;

	InputEvents() = default;
	~InputEvents() = default;
};
