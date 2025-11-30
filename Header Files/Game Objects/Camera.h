#pragma once

#include "../CameraProjection.h"
#include "../Model/BoundingBox.h"
#include "../Model/Transform.h"
#include "IGameObject.h"
#include <glm/glm.hpp>
#include <memory>

using namespace glm;
using namespace std;

class Camera : public IGameObject
{
public:
	// Singleton access.
	static Camera *I();

	Camera();
	~Camera() = default;

	void update(const float deltaTime) override;
	void render() override;

private:
	static unique_ptr<Camera> instance;

	const float fovY = 45.0f;
	const float nearPlane = 0.1f;
	const float farPlane = 2000.0f;

	CameraTransform transform;
	CameraProjection projection;

	unique_ptr<BoundingBox> boundingBox;

	fmat4 makeProjectionMatrix() const;
	void setTransform();
	void setProjectionData();
};
