#include "../Header Files/ButtonStates.h"
#include "../Header Files/InputEvents.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void InputEvents::keyCallback(const GLFWwindow *window, const int key, const int scancode, const int action, const int mods)
{
}

void InputEvents::cursorPositionCallback(const GLFWwindow *window, const double xpos, const double ypos)
{
}

void InputEvents::framebufferSizeCallback(const GLFWwindow *window, const int width, const int height)
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
