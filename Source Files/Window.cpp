#include "../Header Files/Window.h"
#include "../Header Files/InputEvents.h"
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

using namespace std;

optional<unique_ptr<Window>> Window::instance = nullopt;

Window *Window::I()
{
	if (!Window::instance.has_value())
	{
		Window::instance = unique_ptr<Window>(new Window());
	}
	return Window::instance.value().get();
}

Window::Window()
{
	this->initializeWindow();
	this->initInputEvents();
	this->initializeGui();
	this->initOpenGL();
}

Window::~Window()
{
	// Free ImGui resources
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Destroy window and terminate GLFW
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(this->window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

ivec2 Window::getSize()
{
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	return ivec2(width, height);
}

fvec2 Window::getWindowCenterCoordinates()
{
	const ivec2 windowSize = this->getSize();
	return fvec2(windowSize.x / 2.0f, windowSize.y / 2.0f);
}

void Window::centerCursor()
{
	const fvec2 centerCoordinates = this->getWindowCenterCoordinates();
	glfwSetCursorPos(window, centerCoordinates.x, centerCoordinates.y);
}

void Window::disableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::showCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::initializeWindow()
{
	// If glfwInit fails, throw an exception.
	if (!glfwInit())
	{
		cerr << "Failed to initialize GLFW" << endl;
		throw runtime_error("Failed to initialize GLFW");
	}

	// Set the context version and profile properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable double buffering
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();

	// Gets the resolution of the monitor
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	// Sets the window dimensions to a fixed game size
	const int windowWidth = 1920;
	const int windowHeight = 1080;

	// Create a window and its OpenGL context with the fixed dimensions.
	this->window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		cerr << "Failed to create GLFW window" << endl;
		throw runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowAspectRatio(window, windowWidth, windowHeight);

	/*
	 * Creates the current context and associates it to window.
	 * In OpenGL a rendering context is a state machine that stores all the necessary information and resources for graphic rendering.
	 */
	glfwMakeContextCurrent(window);

	// Check if GLAD is able to load the OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		cerr << "Failed to load OpenGL function pointers" << endl;
		throw runtime_error("Failed to load OpenGL function pointers");
	}

	// Resize window to fraction of monitor size
	// const int newWidth = static_cast<int>(mode->width * this->monitorPercentageSize.x);
	// const int newHeight = static_cast<int>(mode->height * this->monitorPercentageSize.y);
	// glfwSetWindowSize(this->window, newWidth, newHeight);

	glViewport(0, 0, windowWidth, windowHeight);

	// Enable V-Sync (Frame rate is capped to monitor refresh rate).
	glfwSwapInterval(1);
}

void Window::initInputEvents()
{
	glfwSetKeyCallback(window, InputEvents::keyCallback);
	glfwSetCursorPosCallback(window, InputEvents::cursorPositionCallback);
	glfwSetFramebufferSizeCallback(window, InputEvents::framebufferSizeCallback);
}

void Window::initializeGui()
{
	// Check ImGui compatibility
	IMGUI_CHECKVERSION();
	// Create ImGui context
	ImGui::CreateContext();
	// Get ImGui IO object for configuration and input handling
	ImGuiIO &io = ImGui::GetIO();
	// Sets global font scale
	io.FontGlobalScale = 2.0f;
	// Set GUI color style
	ImGui::StyleColorsDark();

	// Initializes ImGui integration with GLFW for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	// Sets OpenGL version for ImGui
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Window::initOpenGL()
{
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}
