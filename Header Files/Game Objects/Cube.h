#pragma once
#include "IGameObject.h"
#include <memory>

using namespace std;

class Mesh;

/**
 * @brief Represents a background gameObject in the game.
 * Doesn't have a Bounding Box.
 */
class Cube : public IGameObject
{
public:
	Cube() = default;
	~Cube() = default;

	void update(float deltaTime) override;
	void render() override;

private:
	shared_ptr<Mesh> mesh;
};