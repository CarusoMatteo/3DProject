#include "../../Header Files/CameraProjection.h"
#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

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
}

void Camera::render()
{
	return;
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

void Camera::setTransform()
{
	const fvec3 position = fvec3(0);
	const fvec3 target = fvec3(0, 0, -1);
	const fvec3 up = fvec3(0, 1, 0);
	const fvec3 direction = fvec3(0, 0, -1);

	this->transform = {position, target, up, direction};
}

void Camera::setProjectionData()
{
	const ivec2 windowSize = Window::I()->getSize();
	const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

	this->projection = CameraProjection{
		this->fovY,
		aspect,
		this->nearPlane,
		this->farPlane};
}
