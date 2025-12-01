#include "../Header Files/ButtonStates.h"
#include "../Header Files/InputEvents.h"
#include "../Header Files/InputEventsType.h"
#include "../Header Files/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

unique_ptr<ButtonStates> InputEvents::buttonStates = make_unique<ButtonStates>();
unique_ptr<fvec2> InputEvents::cursorPosition = make_unique<fvec2>(-1.0f);

ButtonStates InputEvents::getButtonStates()
{
	return *InputEvents::buttonStates;
}

fvec2 InputEvents::getCursorPosition()
{
	return *InputEvents::cursorPosition;
}

void InputEvents::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::FORWARD) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::FORWARD) = false;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::BACKWARD) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::BACKWARD) = false;
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::LEFT) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::LEFT) = false;
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::RIGHT) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::RIGHT) = false;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::UP) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::UP) = false;
		break;
	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_C:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::DOWN) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::DOWN) = false;
		break;
	default:
		break;
	}
}

void InputEvents::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
	// Screen resolution
	ivec2 screenSize = Window::I()->getSize();

	cursorPosition->x = static_cast<float>(xpos);
	// Invert y-coordinate to match OpenGL's coordinate system
	cursorPosition->y = static_cast<float>(screenSize.y - ypos);
}

void InputEvents::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
