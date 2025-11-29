#pragma once
#include "IGameObject.h"
#include <memory>

using namespace std;

class Mesh;

/**
 * @brief Represents a background gameObject in the game.
 * Doesn't have a Bounding Box.
 */
class Camera : public IGameObject
{
public:
	Camera() = default;
	~Camera() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	unique_ptr<Mesh> mesh;
};