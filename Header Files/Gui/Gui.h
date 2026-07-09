#pragma once

#include "../Game Objects/IVisibleGameObject.h"
#include "../Model/Transform.h"
#include "IGui.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

constexpr int NONE = -1;

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
	const fvec2 inspectorWindowPosition = fvec2(1000, 10);

	shared_ptr<fvec3> clearColor;
	int selectedObjectIndex = NONE;
	int selectedMeshIndex = NONE;

	shared_ptr<Mesh> lastMesh = nullptr;

	Transform activeTransform;

	void settingsWindow();
	void inspectorWindow(const vector<shared_ptr<IVisibleGameObject>> objects);
	void meshInspectorWindow(const shared_ptr<Mesh> mesh);
	void transformEditorSection(const shared_ptr<Mesh> mesh);
};
