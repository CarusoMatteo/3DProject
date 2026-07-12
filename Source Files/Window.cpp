#include "../Header Files/Window.h"
#include "../Header Files/InputEvents.h"
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>

unique_ptr<Window> Window::instance = nullptr;

Window *Window::I()
{
	if (Window::instance == nullptr)
	{
		Window::instance = unique_ptr<Window>(new Window());
	}
	return Window::instance.get();
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

void Window::disableAndCenterCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	const ivec2 windowSize = this->getSize();
	glfwSetCursorPos(window, windowSize.x / 2.0, windowSize.y / 2.0);
}

void Window::freeCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::initializeWindow()
{
	// If glfwInit fails, throw an exception.
	if (!glfwInit())
	{
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
		throw new runtime_error("Failed to load OpenGL function pointers");
	}

	// Choose cursor mode (normal, hidden, disabled, infinte), and its initial position
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// glfwSetCursorPos(window, 0, 0);

	// Resize window to fraction of monitor size
	const int newWidth = static_cast<int>(mode->width * this->monitorPercentageSize.x);
	const int newHeight = static_cast<int>(mode->height * this->monitorPercentageSize.y);
	glfwSetWindowSize(this->window, newWidth, newHeight);

	glViewport(0, 0, newWidth, newHeight);

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}
