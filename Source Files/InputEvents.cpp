#include "../Header Files/ButtonStates.h"
#include "../Header Files/InputEvents.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void InputEvents::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

void InputEvents::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
}

void InputEvents::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
}

ButtonStates *InputEvents::getButtonStates()
{
	return nullptr;
}

dvec2 *InputEvents::getCursorPosition()
{
	return nullptr;
}

ivec2 InputEvents::getCurrentWindowSize()
{
	return ivec2();
}

void InputEvents::setCurrentWindowSize(ivec2 newSize)
{
}
