#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Gui/IGui.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/InputEventsType.h"
#include "../../Header Files/Lights/LightManager.h"
#include "../../Header Files/Model/Mesh.h"
#include "../../Header Files/Renderers/Renderer.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

Gui::Gui(const shared_ptr<fvec3> clearColor) : IGui()
{
	this->clearColor = clearColor;
}

void Gui::drawGui(vector<shared_ptr<IVisibleGameObject>> objects)
{
	// Hide UI when we're about to take a screenshot or when UI is disabled
	if (InputEvents::shouldTakeScreenshotNextFrame(false) || InputEvents::getButtonStates().at(InputEventsType::DISABLE_UI))
		return;

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
	if (this->selectedObjectIndex.has_value())
		objectInspectorWindow(objects.at(*this->selectedObjectIndex));
	if (this->selectedObjectIndex.has_value() && this->selectedMeshIndex.has_value())
		meshInspectorWindow(objects.at(*this->selectedObjectIndex)->getMeshes().at(*this->selectedMeshIndex));

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

	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

	ImGui::ColorEdit3("Background Color", (float *)this->clearColor.get());

	fvec2 mousePosition = InputEvents::getCursorPosition();
	const CameraFullTransform cameraTransform = Camera::I()->getTransform();
	const fvec3 cameraPosition = cameraTransform.position;

	ImGui::Text("Mouse Coordinates relative to GLFW Window: (%.1f, %.1f)", mousePosition.x, mousePosition.y);
	ImGui::Checkbox("Wireframe", Renderer::getDrawWireframeFlag().get());
	ImGui::Text("Camera position: (%.1f, %.1f, %.1f), target: (%.1f, %.1f, %.1f)", cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraTransform.target.x, cameraTransform.target.y, cameraTransform.target.z);

	fvec3 *lightPositionPtr = LightManager::I()->getPositionPtrs()[0];
	ImGui::SliderFloat("Light position x", &lightPositionPtr->x, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position y", &lightPositionPtr->y, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position z", &lightPositionPtr->z, -50.0f, 50.0f);

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

	ImGui::SeparatorText("Controls");
	ImGui::Text("WASD: Move camera horizontally");
	ImGui::Text("QE: Move camera up/down");
	ImGui::Text("TAB: %s", InputEvents::getButtonStates().at(InputEventsType::FREE_CURSOR) ? "Enable camera movement" : "Free cursor");
	ImGui::Text("P: Take buffer screenshots");
	ImGui::Text("K: Delete screenshots in img directory");
	ImGui::Text("U: Toggle UI");
	ImGui::Text("1~4: Change render mode (1: Main, 2: Normal, 3: Depth, 4: Velocity)");
	ImGui::End();
}

void Gui::inspectorWindow(const vector<shared_ptr<IVisibleGameObject>> objects)
{
	ImGui::SetNextWindowPos(ImVec2(this->inspectorWindowPosition.x, this->inspectorWindowPosition.y));

	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		char label[64];
		sprintf_s(label, sizeof(label), "%d. %s", i + 1, objects.at(i)->getName().c_str());
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

		bool nodeOpen = ImGui::TreeNodeEx(label, nodeFlags);

		// Click on the object (non on a specific mesh)
		if (ImGui::IsItemClicked())
		{
			this->selectedObjectIndex = i;
			this->selectedMeshIndex = nullopt;
		}

		if (nodeOpen)
		{
			for (unsigned int j = 0; j < objects.at(i)->getMeshes().size(); ++j)
			{
				shared_ptr<Mesh> mesh = objects.at(i)->getMeshes().at(j);
				char meshLabel[128];
				sprintf_s(meshLabel, sizeof(meshLabel), "%d.%d. %s", i + 1, j + 1, mesh->getName().empty() ? "Mesh" : mesh->getName().c_str());

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

void Gui::objectInspectorWindow(const shared_ptr<IVisibleGameObject> object)
{
	ImGui::Separator();
	// Unique ID to avoid conflicts
	ImGui::PushID(&object);

	ImGui::Text("Object: %s", object->getName().c_str());
	objectTransformEditorSection(object);

	ImGui::PopID();
}

void Gui::meshInspectorWindow(const shared_ptr<Mesh> mesh)
{
	ImGui::Separator();
	// Unique ID to avoid conflicts
	ImGui::PushID(&mesh);

	ImGui::Text("Mesh: %s", mesh->getName().c_str());
	meshTransformEditorSection(mesh);

	ImGui::PopID();
}

void Gui::objectTransformEditorSection(const shared_ptr<IVisibleGameObject> object)
{
	if (this->lastObject != object)
	{
		// Load new Transform
		this->activeObjectTransform = object->getTransform();
		this->lastObject = object;
	}

	bool transformChanged = false;

	ImGui::Text("Position");
	transformChanged |= ImGui::DragFloat3("Position", value_ptr(this->activeObjectTransform.position), 0.1f, -50.0f, 50.0f);

	ImGui::Text("Rotation");
	transformChanged |= ImGui::DragFloat("Angle", &this->activeObjectTransform.rotation.angle, 0.1f, -360.0f, 360.0f);
	transformChanged |= ImGui::DragFloat3("Axis", value_ptr(this->activeObjectTransform.rotation.axis), 0.1f, -1.0f, 1.0f);

	ImGui::Text("Scale");
	transformChanged |= ImGui::DragFloat3("Scale", value_ptr(this->activeObjectTransform.scale), 0.1f, -50.0f, 50.0f);

	if (transformChanged)
	{
		object->setTransform(this->activeObjectTransform);
	}
}

void Gui::meshTransformEditorSection(const shared_ptr<Mesh> mesh)
{
	if (this->lastMesh != mesh)
	{
		// Load new Transform
		this->activeMeshTransform = mesh->getTransform();
		this->lastMesh = mesh;
	}

	bool transformChanged = false;

	ImGui::Text("Position");
	transformChanged |= ImGui::DragFloat3("Position", value_ptr(this->activeMeshTransform.position), 0.1f, -50.0f, 50.0f);

	ImGui::Text("Rotation");
	transformChanged |= ImGui::DragFloat("Angle", &this->activeMeshTransform.rotation.angle, 0.1f, -360.0f, 360.0f);
	transformChanged |= ImGui::DragFloat3("Axis", value_ptr(this->activeMeshTransform.rotation.axis), 0.1f, -1.0f, 1.0f);

	ImGui::Text("Scale");
	transformChanged |= ImGui::DragFloat3("Scale", value_ptr(this->activeMeshTransform.scale), 0.1f, -50.0f, 50.0f);

	if (transformChanged)
	{
		mesh->setTransform(this->activeMeshTransform);
	}
}
