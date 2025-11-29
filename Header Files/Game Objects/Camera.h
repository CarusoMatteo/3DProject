#pragma once
#include "../Model/BoundingBox.h"
#include "../ProjectionData.h"
#include "IGameObject.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

class Camera : public IGameObject
{
public:
	Camera();
	~Camera() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	fvec3 position = fvec3(0);
	fvec3 target = fvec3(0, 0, -1);
	fvec3 up = fvec3(0, 1, 0);
	fvec3 direction = fvec3(0, 0, -1);
	ProjectionData projectionData;

	unique_ptr<BoundingBox> boundingBox;
};