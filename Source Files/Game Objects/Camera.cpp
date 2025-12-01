#include "../../Header Files/CameraProjection.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/InputEventsType.h"
#include "../../Header Files/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

using namespace glm;
using namespace std;

unique_ptr<Camera> Camera::instance = nullptr;

Camera *Camera::I()
{
	if (Camera::instance == nullptr)
	{
		Camera::instance = unique_ptr<Camera>(new Camera());
	}
	return Camera::instance.get();
}

Camera::Camera()
{
	this->setTransform();
	this->setProjectionData();
}

void Camera::update(float deltaTime)
{
	fvec3 movementDirection = fvec3(0);
	if (InputEvents::getButtonStates().at(InputEventsType::FORWARD))
		movementDirection.z -= 1.0f;
	if (InputEvents::getButtonStates().at(InputEventsType::BACKWARD))
		movementDirection.z += 1.0f;
	if (InputEvents::getButtonStates().at(InputEventsType::LEFT))
		movementDirection.x -= 1.0f;
	if (InputEvents::getButtonStates().at(InputEventsType::RIGHT))
		movementDirection.x += 1.0f;
	if (InputEvents::getButtonStates().at(InputEventsType::UP))
		movementDirection.y += 1.0f;
	if (InputEvents::getButtonStates().at(InputEventsType::DOWN))
		movementDirection.y -= 1.0f;

	if (movementDirection != fvec3(0.0f))
		movementDirection = normalize(movementDirection);

	this->transform.position += movementDirection * speed * deltaTime;
}

fmat4 Camera::makeProjectionMatrix() const
{
	const ivec2 windowSize = Window::I()->getSize();
	const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

	return perspective(radians(this->projection.fovY), aspect, this->projection.nearPlane, this->projection.farPlane);
}

fmat4 Camera::makeViewMatrix() const
{
	return this->transform.toMatrix();
}

fvec3 Camera::getPosition() const
{
	return this->transform.position;
}

void Camera::setTransform()
{
	const fvec3 position = fvec3(1.0f, 1.0f, 1.0f) * 2.0f;
	const fvec3 target = fvec3(0);
	const fvec3 up = fvec3(0, 1, 0);
	const fvec3 direction = target - position;

	this->transform = {position, target, up, direction};
}

void Camera::setProjectionData()
{
	const float fovY = 45.0f;
	const float nearPlane = 0.1f;
	const float farPlane = 2000.0f;

	const ivec2 windowSize = Window::I()->getSize();
	const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

	this->projection = CameraProjection{
		fovY,
		aspect,
		nearPlane,
		farPlane};
}
