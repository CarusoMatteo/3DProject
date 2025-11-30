#pragma once

#include "../Model/Model.h"
#include "IGameObject.h"
#include <memory>

using namespace std;

class Mesh;

class Cube : public IGameObject
{
public:
	Cube();
	~Cube() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	shared_ptr<Model> model;
};