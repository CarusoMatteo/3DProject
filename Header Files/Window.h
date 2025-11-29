#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>

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

private:
	const string windowTitle = "Progetto 3D";
	const fvec2 monitorPercentageSize = fvec2(0.8f);
	static unique_ptr<Window> instance;

	Window();

	// Can't be smart pointer.
	GLFWwindow *window;

	void initializeWindow();
	void initInputEvents();
	void initializeGui();
	void initOpenGL();
	static fmat4 createProjectionMatrix(int windowWidth, int windowHeight);
};
