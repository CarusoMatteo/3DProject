#pragma once
#include "IGameObject.h"

using namespace std;

class Camera : public IGameObject
{
public:
	Camera() = default;
	~Camera() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	// unique_ptr<Mesh> mesh;
};