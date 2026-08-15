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
	static Camera *I(vector<CameraTransform> states);

	~Camera() = default;

	void update(const float deltaTime) override;
	fmat4 makeProjectionMatrix() const;
	fmat4 makeViewMatrix() const;
	fvec3 getPosition() const;

private:
	static optional<unique_ptr<Camera>> instance;

	const float speed = 2.5f;
	const float rotationSpeed = 0.05f;

	CameraTransform transform;
	CameraProjection projection;

	unique_ptr<BoundingBox> boundingBox;
	vector<CameraTransform> states;

	unsigned int currentState = 0;
	float lerpProgress = 0;
	const float lerpSpeed = 0.7f;

	Camera(vector<CameraTransform> states);
	void moveFirstPerson(float deltaTime);
	void panFirstPerson();
	void setProjectionData();
};
