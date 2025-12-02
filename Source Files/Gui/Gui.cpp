#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Gui/IGui.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/Model/Shader.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <memory>

using namespace glm;
using namespace std;

Gui::Gui(const shared_ptr<fvec3> clearColor) : IGui()
{
	this->clearColor = clearColor;
}

void Gui::drawGui()
{
	// Prepares a new frame for ImGui
	ImGui_ImplGlfw_NewFrame();
	// Prepares a new frame for OpenGL3 rendering
	ImGui_ImplOpenGL3_NewFrame();
	// Starts a new recording for a ImGui frame
	ImGui::NewFrame();
	// Disables the ini file saving/loading.
	ImGui::GetIO().IniFilename = nullptr;

	settingsWindow();

	// Ends the ImGui frame declaration.
	ImGui::End();
	// Renders the ImGui frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const shared_ptr<fvec3> Gui::getClearColor()
{
	return this->clearColor;
}

void Gui::settingsWindow()
{
	// Sets the position for the next window.
	ImGui::SetNextWindowPos(ImVec2(this->settingsWindowPosition.x, this->settingsWindowPosition.y));

	ImGui::Begin("Impostazioni", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	ImGui::ColorEdit3("Colore di Sfondo", (float *)this->clearColor.get());

	fvec2 mousePosition = InputEvents::getCursorPosition();

	ImGui::Text("Coordinate Mouse relative alla finestra GLFW: (%.1f, %.1f)", mousePosition.x, mousePosition.y);
	ImGui::Checkbox("Wireframe", Shader::getDrawWireframeFlag().get());
	ImGui::Checkbox("Ancora", Shader::getDrawAnchorFlag().get());
	// ImGui::Checkbox("Bounding Box", MeshBB::shouldDrawBoundingBoxRef());

	fvec3 *lightPositionPtr = PointLight::I()->getPositionPtr();
	ImGui::SliderFloat("Light position x", &lightPositionPtr->x, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position y", &lightPositionPtr->y, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position z", &lightPositionPtr->z, -50.0f, 50.0f);

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}
