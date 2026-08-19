#pragma once

#include "../Game Objects/IVisibleGameObject.h"
#include "../Model/Transform.h"
#include "IGui.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <vector>

using namespace glm;
using namespace std;

class Mesh;

/**
 * @brief Represents the main menu GUI of the application.
 */
class Gui : public IGui
{
public:
	Gui(const shared_ptr<fvec3> clearColor);

	void drawGui(vector<shared_ptr<IVisibleGameObject>> objects) override;
	const shared_ptr<fvec3> getClearColor() override;

private:
	const fvec2 settingsWindowPosition = fvec2(10, 10);
	const fvec2 inspectorWindowPosition = fvec2(10, 570);

	shared_ptr<fvec3> clearColor;

	optional<int> selectedObjectIndex = nullopt;
	optional<int> selectedMeshIndex = nullopt;

	shared_ptr<IVisibleGameObject> lastObject = nullptr;
	shared_ptr<Mesh> lastMesh = nullptr;

	Transform activeObjectTransform;
	Transform activeMeshTransform;

	void settingsWindow();
	void inspectorWindow(const vector<shared_ptr<IVisibleGameObject>> objects);
	void objectInspectorWindow(const shared_ptr<IVisibleGameObject> object);
	void meshInspectorWindow(const shared_ptr<Mesh> mesh);
	void objectTransformEditorSection(const shared_ptr<IVisibleGameObject> object);
	void meshTransformEditorSection(const shared_ptr<Mesh> mesh);
};
