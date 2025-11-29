#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/InputEvents.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

Gui::Gui(const fvec3 *clearColorRef) : IGui()
{
	this->clearColorRef = clearColorRef;
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

	settingsWindow(fvec2(10, 10));

	// Ends the ImGui frame declaration.
	ImGui::End();
	// Renders the ImGui frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const fvec3 *Gui::getClearColorRef()
{
	return this->clearColorRef;
}

void Gui::settingsWindow(const fvec2 position)
{
	dvec2 *mousePosition = InputEvents::getCursorPosition();

	// Sets the position for the next window.
	ImGui::SetNextWindowPos(ImVec2(position.x, position.y));

	ImGui::Begin("Impostazioni", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	ImGui::ColorEdit3("Colore di Sfondo", (float *)this->clearColorRef);
	ImGui::Text("Coordinate Mouse relative alla finestra GLFW: (%.1f, %.1f)", mousePosition->x, mousePosition->y);
	// ImGui::Checkbox("Wireframe", Mesh::getIsWireframeRef());
	// ImGui::Checkbox("Bounding Box", MeshBB::shouldDrawBoundingBoxRef());
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}
