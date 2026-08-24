#pragma once

#include "../Camera/CameraProjection.h"
#include "../Game Objects/IGameObject.h"
#include "../Model/BoundingBox.h"
#include "../Model/Transform.h"
#include "../Path/CameraPath.h"
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
	static Camera *I(CameraFullTransform transform);
	static Camera *I(CameraPath cameraPath);

	~Camera() = default;

	void update(const float deltaTime) override;
	fmat4 makeProjectionMatrix() const;
	fmat4 makeViewMatrix() const;
	CameraFullTransform getTransform() const;

private:
	static optional<unique_ptr<Camera>> instance;

	const float speed = 2.5f;
	const float rotationSpeed = 0.05f;

	CameraFullTransform transform;
	CameraProjection projection;

	unique_ptr<BoundingBox> boundingBox;
	optional<CameraPath> cameraPath;

	Camera(CameraFullTransform transform);
	Camera(CameraPath cameraPath);
	void moveFirstPerson(float deltaTime);
	void panFirstPerson();
	void setProjectionData();
};
