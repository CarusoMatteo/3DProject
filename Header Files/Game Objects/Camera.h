#pragma once

#include "../CameraProjection.h"
#include "../Model/BoundingBox.h"
#include "../Model/Transform.h"
#include "IGameObject.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>

using namespace glm;
using namespace std;

class Camera : public IGameObject
{
public:
	// Singleton access.
	static Camera *I();

	~Camera() = default;

	void update(const float deltaTime) override;
	fmat4 makeProjectionMatrix() const;
	fmat4 makeViewMatrix() const;
	fvec3 getPosition() const;

private:
	static optional<unique_ptr<Camera>> instance;

	const float speed = 2.5f;
	const float rotationSpeed = 0.05f;
	Camera();

	CameraTransform transform;
	CameraProjection projection;

	unique_ptr<BoundingBox> boundingBox;

	void moveFirstPerson(float deltaTime);
	void panFirstPerson();
	void setTransform();
	void setProjectionData();
};
