#pragma once
#include "IGameObject.h"
#include <memory>

using namespace std;

class Mesh;

class Cube : public IGameObject
{
public:
	Cube() = default;
	~Cube() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	unique_ptr<Mesh> mesh;
};