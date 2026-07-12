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
	moveFirstPerson(deltaTime);
	if (!InputEvents::getButtonStates().at(InputEventsType::FREE_CURSOR))
		panFirstPerson(deltaTime);
	else
		Window::I()->freeCursor();
}

/*
void Camera::moveAlongAxes(float deltaTime)
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
*/

void Camera::moveFirstPerson(float deltaTime)
{
	fvec3 movementDirection = fvec3(0);
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

void Camera::panFirstPerson(float deltaTime)
{
	const fvec2 centerCoordinates = fvec2(Window::I()->getSize().x / 2.0f, Window::I()->getSize().y / 2.0f);
	const fvec2 cursorPosition = InputEvents::getCursorPosition();
	const fvec2 offset = cursorPosition - centerCoordinates;

	static float yaw = 0.0f;
	static float pitch = 0.0f;
	static bool initialized = false;

	if (!initialized)
	{
		const fvec3 forwardDirection = normalize(this->transform.target - this->transform.position);
		yaw = degrees(atan2(forwardDirection.z, forwardDirection.x));
		pitch = degrees(asin(forwardDirection.y));
		initialized = true;
	}

	yaw += offset.x * this->rotationSpeed;
	pitch -= offset.y * this->rotationSpeed;

	pitch = clamp(pitch, -89.0f, 89.0f);

	fvec3 front = fvec3(0);
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));

	this->transform.direction = normalize(front);
	this->transform.target = this->transform.position + this->transform.direction;

	Window::I()->disableAndCenterCursor();
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
	fvec3 position = fvec3(0, 0, 5);
	fvec3 target = fvec3(0);
	fvec3 up = fvec3(0, 1, 0);
	fvec3 direction = target - position;

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
