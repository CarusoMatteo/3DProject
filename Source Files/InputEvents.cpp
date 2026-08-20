#include "../Header Files/InputEvents.h"
#include "../Header Files/ButtonStates.h"
#include "../Header Files/InputEventsType.h"
#include "../Header Files/Renderers/RenderMode.h"
#include "../Header Files/Renderers/Renderer.h"
#include "../Header Files/Window.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <string>

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
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::FORWARD) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::FORWARD) = false;
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::BACKWARD) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::BACKWARD) = false;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::LEFT) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::LEFT) = false;
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::RIGHT) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::RIGHT) = false;
		break;
	case GLFW_KEY_E:
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::UP) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::UP) = false;
		break;
	case GLFW_KEY_C:
	case GLFW_KEY_Q:
	case GLFW_KEY_LEFT_SHIFT:
		if (action == GLFW_PRESS)
			buttonStates->at(InputEventsType::DOWN) = true;
		else if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::DOWN) = false;
		break;
	case GLFW_KEY_TAB:
		if (action == GLFW_PRESS)
		{
			buttonStates->at(InputEventsType::FREE_CURSOR) = !buttonStates->at(InputEventsType::FREE_CURSOR);
			Window::I()->centerCursor();

			if (buttonStates->at(InputEventsType::FREE_CURSOR))
				Window::I()->showCursor();
			else
				Window::I()->disableCursor();
		}
		break;
	case GLFW_KEY_P:
		// Happens on release so we're sure it only happens once.
		if (action == GLFW_RELEASE)
			buttonStates->at(InputEventsType::TAKE_SCREENSHOT_NEXT_FRAME) = true;
		break;
	case GLFW_KEY_K:
		if (action == GLFW_RELEASE)
			InputEvents::deleteScreenshotsFiles();
		break;
	case GLFW_KEY_1:
		if (action == GLFW_RELEASE)
			Renderer::setRenderMode(RenderMode::MAIN);
		break;
	case GLFW_KEY_2:
		if (action == GLFW_RELEASE)
			Renderer::setRenderMode(RenderMode::NORMAL);
		break;
	case GLFW_KEY_3:
		if (action == GLFW_RELEASE)
			Renderer::setRenderMode(RenderMode::DEPTH);
		break;
	case GLFW_KEY_4:
		if (action == GLFW_RELEASE)
			Renderer::setRenderMode(RenderMode::VELOCITY);
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

bool InputEvents::shouldTakeScreenshotNextFrame(bool consumeInput)
{
	if (buttonStates->at(InputEventsType::TAKE_SCREENSHOT_NEXT_FRAME))
	{
		if (consumeInput)
			buttonStates->at(InputEventsType::TAKE_SCREENSHOT_NEXT_FRAME) = false;
		return true;
	}
	return false;
}

void InputEvents::deleteScreenshotsFiles()
{
	const string directoryPath = "img";

	if (!filesystem::exists(directoryPath))
	{
		cerr << "Directory \"" << directoryPath << "\" doesn't exist." << endl;
		return;
	}
	if (!filesystem::is_directory(directoryPath))
	{
		cerr << "\"" << directoryPath << "\" is not a directory." << endl;
		return;
	}

	// Iterate on all files of the directory
	for (const auto &entry : filesystem::directory_iterator(directoryPath))
	{
		// Check if it's a regular file and has the .bmp or .png extension
		if (entry.is_regular_file() && (entry.path().extension() == ".bmp" || entry.path().extension() == ".png"))
		{
			filesystem::remove(entry.path());
			cout << "Deleted image: " << entry.path().filename() << endl;
		}
	}
}
