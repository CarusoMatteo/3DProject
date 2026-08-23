#include "../../Header Files/Camera/Camera.h"
#include "../../Header Files/Camera/CameraPath.h"
#include "../../Header Files/Camera/CameraProjection.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/InputEventsType.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Window.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <optional>

using namespace glm;
using namespace std;

optional<unique_ptr<Camera>> Camera::instance = nullopt;

Camera *Camera::I()
{
	return Camera::I(CameraTransform{.position = fvec3(0, 0, -10), .target = fvec3(0)}.toCameraTransform());
}

Camera *Camera::I(CameraFullTransform transform)
{
	if (!Camera::instance.has_value())
	{
		Camera::instance = unique_ptr<Camera>(new Camera(transform));
	}
	return Camera::instance.value().get();
}

Camera *Camera::I(CameraPath cameraPath)
{
	if (!Camera::instance.has_value())
	{
		Camera::instance = unique_ptr<Camera>(new Camera(cameraPath));
	}
	return Camera::instance.value().get();
}

Camera::Camera(CameraFullTransform transform) : transform(transform)
{
	this->cameraPath = nullopt;
	this->setProjectionData();

	// Choose cursor mode, and its initial position
	Window::I()->centerCursor();
	Window::I()->disableCursor();
}

Camera::Camera(CameraPath cameraPath) : cameraPath(cameraPath)
{
	this->transform = cameraPath.getFirstTransform();
	this->setProjectionData();

	// Choose cursor mode, and its initial position
	Window::I()->centerCursor();
	Window::I()->disableCursor();
}

void Camera::update(float deltaTime)
{
	if (!this->cameraPath.has_value())
	{
		moveFirstPerson(deltaTime);
		if (!InputEvents::getButtonStates().at(InputEventsType::FREE_CURSOR))
			panFirstPerson();
		return;
	}

	this->transform = this->cameraPath->getNextTransform(deltaTime);
}

void Camera::moveFirstPerson(float deltaTime)
{
	if (InputEvents::getButtonStates().at(InputEventsType::FORWARD))
	{
		this->transform.direction = this->transform.target - this->transform.position;
		this->transform.position += this->transform.direction * this->speed * deltaTime;
		this->transform.target = this->transform.position + this->transform.direction;
	}
	if (InputEvents::getButtonStates().at(InputEventsType::BACKWARD))
	{
		this->transform.direction = this->transform.target - this->transform.position;
		this->transform.position -= this->transform.direction * this->speed * deltaTime;
		this->transform.target = this->transform.position + this->transform.direction;
	}
	if (InputEvents::getButtonStates().at(InputEventsType::LEFT))
	{
		this->transform.direction = this->transform.target - this->transform.position;
		fvec3 slide_vector = cross(this->transform.direction, this->transform.up) * this->speed * deltaTime; // Perpendicular to direction and up
		this->transform.position -= slide_vector;
		this->transform.target -= slide_vector;
	}
	if (InputEvents::getButtonStates().at(InputEventsType::RIGHT))
	{
		this->transform.direction = this->transform.target - this->transform.position;
		fvec3 slide_vector = cross(this->transform.direction, this->transform.up) * this->speed * deltaTime; // Perpendicular to direction and up
		this->transform.position += slide_vector;
		this->transform.target += slide_vector;
	}
	if (InputEvents::getButtonStates().at(InputEventsType::UP))
	{
		this->transform.direction = this->transform.target - this->transform.position;
		fvec3 slide_vector = normalize(cross(this->transform.direction, this->transform.up));		  // Perpendicular to direction and side
		fvec3 upDirection = cross(this->transform.direction, slide_vector) * this->speed * deltaTime; // Up vector with inverse cross product
		this->transform.position -= upDirection;
		this->transform.target -= upDirection;
	}
	if (InputEvents::getButtonStates().at(InputEventsType::DOWN))
	{
		this->transform.direction = this->transform.target - this->transform.position;
		fvec3 slide_vector = normalize(cross(this->transform.direction, this->transform.up));
		fvec3 upDirection = cross(this->transform.direction, slide_vector) * this->speed * deltaTime;
		this->transform.position += upDirection;
		this->transform.target += upDirection;
	}
}

void Camera::panFirstPerson()
{
	const fvec2 cursorPosition = InputEvents::getCursorPosition();
	static fvec2 previousPosition = cursorPosition;
	const fvec2 offset = cursorPosition - previousPosition;

	static float yaw = 0.0f;
	static float pitch = 0.0f;

	yaw += offset.x * this->rotationSpeed;
	pitch += offset.y * this->rotationSpeed;

	pitch = clamp(pitch, -89.0f, 89.0f);

	fvec3 front = fvec3(0);
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));

	this->transform.direction = normalize(front);
	this->transform.target = this->transform.position + this->transform.direction;

	previousPosition = cursorPosition;
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

CameraFullTransform Camera::getTransform() const
{
	return this->transform;
}

void Camera::setProjectionData()
{
	const float fovY = 45.0f;
	const float nearPlane = 0.1f;
	const float farPlane = 1000.0f;

	const ivec2 windowSize = Window::I()->getSize();
	const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

	this->projection = CameraProjection{
		fovY,
		aspect,
		nearPlane,
		farPlane};
}
