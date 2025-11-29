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
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
	static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
	// Add more callbacks as needed here

	static ButtonStates *getButtonStates();
	static dvec2 *getCursorPosition();
	static ivec2 getCurrentWindowSize();
	static void setCurrentWindowSize(ivec2 newSize);

private:
	static ButtonStates buttonStates;
	static dvec2 cursorPosition;
	static ivec2 currentWindowSize;

	InputEvents() = default;
	~InputEvents() = default;
};
