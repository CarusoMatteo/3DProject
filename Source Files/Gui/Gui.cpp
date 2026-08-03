#include "../../Header Files/Gui/Gui.h"
#include "../../Header Files/Game Objects/IVisibleGameObject.h"
#include "../../Header Files/Game Objects/PointLight.h"
#include "../../Header Files/Gui/IGui.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/InputEventsType.h"
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

	ImGui::Text("Mouse Coordinates relative to GLFW Window: (%.1f, %.1f)", mousePosition.x, mousePosition.y);
	ImGui::Checkbox("Wireframe", Renderer::getDrawWireframeFlag().get());
	ImGui::Checkbox("Anchor", Renderer::getDrawAnchorFlag().get());
	// ImGui::Checkbox("Bounding Box", MeshBB::shouldDrawBoundingBoxRef());

	fvec3 *lightPositionPtr = PointLight::I()->getPositionPtr();
	ImGui::SliderFloat("Light position x", &lightPositionPtr->x, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position y", &lightPositionPtr->y, -50.0f, 50.0f);
	ImGui::SliderFloat("Light position z", &lightPositionPtr->z, -50.0f, 50.0f);

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

	ImGui::SeparatorText("Controls");
	ImGui::Text("WASD: Move camera horizontallly");
	ImGui::Text("QE: Move camera up/down");
	ImGui::Text("TAB: %s", InputEvents::getButtonStates().at(InputEventsType::FREE_CURSOR) ? "Enable camera movement" : "Free cursor");
	ImGui::End();
}

void Gui::inspectorWindow(const vector<shared_ptr<IVisibleGameObject>> objects)
{
	ImGui::SetNextWindowPos(ImVec2(this->inspectorWindowPosition.x, this->inspectorWindowPosition.y));

	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	for (int i = 0; i < objects.size(); ++i)
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
			for (int j = 0; j < objects.at(i)->getMeshes().size(); ++j)
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
