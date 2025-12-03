#pragma once

#include "../Game Objects/Cube.h"
#include "../Game Objects/Plane.h"
#include "../Game Objects/Sphere.h"
#include "../Gui/Gui.h"
#include "IScene.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

class Scene : public IScene
{
public:
	Scene(const shared_ptr<fvec3> clearColor);
	~Scene() = default;

	void updateGameObjects(const float deltaTime) override;
	void renderScene() override;

private:
	// Do not save. Use singleton access instead
	// unique_ptr<Camera> camera;
	// Do not save. Use singleton access instead
	// unique_ptr<PointLight> light;

	unique_ptr<Plane> plane;
	vector<shared_ptr<Cube>> cubes;
	vector<shared_ptr<Sphere>> spheres;

	unique_ptr<Gui> gui;

	void scatterObjects();
};
