#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Gui/IGui.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Model/Shader.h"
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <memory>
#include <vector>
#include <string>

using namespace glm;
using namespace std;

static char* c_str_array(vector<string> array) {
	static vector<char> buffer;
	buffer.clear();
	for (const auto& str : array) {
		buffer.insert(buffer.end(), str.begin(), str.end());
		buffer.push_back('\0');
	}
	return buffer.data();
}

Gui::Gui(const shared_ptr<fvec3> clearColor) : IGui()
{
	this->clearColor = clearColor;
}

void Gui::drawGui(vector<shared_ptr<IVisibleGameObject>> objects)
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
	inspectorWindow(objects);
	if (this->selectedObjectIndex != NONE && this->selectedMeshIndex != NONE)
		meshInspectorWindow(objects.at(selectedObjectIndex)->getMeshes().at(selectedMeshIndex));

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

	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	ImGui::ColorEdit3("Background Color", (float *)this->clearColor.get());

	fvec2 mousePosition = InputEvents::getCursorPosition();

	ImGui::Text("Mouse Coordinates relative to GLFW Window: (%.1f, %.1f)", mousePosition.x, mousePosition.y);
	ImGui::Checkbox("Wireframe", Shader::getDrawWireframeFlag().get());
	ImGui::Checkbox("Ancora", Shader::getDrawAnchorFlag().get());
	// ImGui::Checkbox("Bounding Box", MeshBB::shouldDrawBoundingBoxRef());

	fvec3 *lightPositionPtr = PointLight::I()->getPositionPtr();
	ImGui::SliderFloat("Light position x", &lightPositionPtr->x, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position y", &lightPositionPtr->y, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position z", &lightPositionPtr->z, -50.0f, 50.0f);

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::End();
}

void Gui::inspectorWindow(const vector<shared_ptr<IVisibleGameObject>> objects)
{
	ImGui::SetNextWindowPos(ImVec2(this->inspectorWindowPosition.x, this->inspectorWindowPosition.y));

	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	for (int i = 0; i < objects.size(); ++i)
	{
		char label[64];
		sprintf_s(label, sizeof(label), "%d. %s", i, objects.at(i)->getName().c_str());
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

		bool nodeOpen = ImGui::TreeNodeEx(label, nodeFlags);

		// Click on the object (non on a specific mesh)
		if (ImGui::IsItemClicked())
		{
			this->selectedObjectIndex = i;
			this->selectedMeshIndex = NONE;
		}

		if (nodeOpen)
		{
			for (int j = 0; j < objects.at(i)->getMeshes().size(); ++j)
			{
				shared_ptr<Mesh> mesh = objects.at(i)->getMeshes().at(j);
				char meshLabel[128];
				sprintf_s(meshLabel, sizeof(meshLabel), "%d.%d. %s", i, j, mesh->getName().empty() ? "Mesh" : mesh->getName().c_str());

				bool isSelected = this->selectedObjectIndex == i && this->selectedMeshIndex == j;
				if (ImGui::Selectable(meshLabel, isSelected))
				{
					this->selectedObjectIndex = i;
					this->selectedMeshIndex = j;
				}
			}
			ImGui::TreePop();
		}
	}
}

void Gui::meshInspectorWindow(const shared_ptr<Mesh> mesh)
{
	// Unique ID to avoid conflicts
	ImGui::PushID(&mesh);

	ImGui::Text("Mesh: %s", mesh->getName().c_str());
	ImGui::Separator();

	transformEditorSection(mesh);

	ImGui::PopID();
}

void Gui::transformEditorSection(const shared_ptr<Mesh> mesh)
{
	if (this->lastMesh != mesh)
	{
		// Load new Transform
		this->activeTransform = mesh->getTransform();
		this->lastMesh = mesh;
	}

	bool transformChanged = false;

	ImGui::Separator();
	ImGui::Text("Position");
	transformChanged |= ImGui::DragFloat3("Position", value_ptr(this->activeTransform.position), 0.1f, -50.0f, 50.0f);

	ImGui::Text("Rotation");
	transformChanged |= ImGui::DragFloat("Angle", &this->activeTransform.rotation.angle, 0.1f, -360.0f, 360.0f);
	transformChanged |= ImGui::DragFloat3("Axis", value_ptr(this->activeTransform.rotation.axis), 0.1f, -1.0f, 1.0f);

	ImGui::Text("Scale");
	transformChanged |= ImGui::DragFloat3("Scale", value_ptr(this->activeTransform.scale), 0.1f, -50.0f, 50.0f);

	if (transformChanged)
	{
		mesh->setTransform(this->activeTransform);
	}
}
