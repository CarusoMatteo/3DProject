#pragma once

#include "../Model/Model.h"
#include "IVisibleGameObject.h"
#include <memory>

using namespace std;

class Mesh;

class Plane : public IVisibleGameObject
{
public:
	Plane();
	~Plane() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	shared_ptr<Model> model;
};
