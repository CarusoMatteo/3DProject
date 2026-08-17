#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/CameraProjection.h"
#include "../../Header Files/InputEvents.h"
#include "../../Header Files/InputEventsType.h"
#include "../../Header Files/Model/Transform.h"
#include "../../Header Files/Window.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <optional>
#include <vector>

using namespace glm;
using namespace std;

optional<unique_ptr<Camera>> Camera::instance = nullopt;

static float easeLinear(const float t)
{
	return t;
}

static float easeInOutSmooth(const float t)
{
	return powf(t, 2) * (3.0f - 2.0f * t);
}

static float easeInOutSmoother(const float t)
{
	return powf(t, 3) * (t * (t * 6.0f - 15.0f) + 10.0f);
}

static fvec3 lerp(const fvec3 a, const fvec3 b, const float t, float (*easingFunction)(const float) = easeLinear)
{
	return a + easingFunction(t) * (b - a);
}

static CameraTransform lerp(const CameraTransform a, const CameraTransform b, const float t, float (*easingFunction)(const float) = easeLinear)
{
	return {
		lerp(a.position, b.position, t, easingFunction),
		lerp(a.target, b.target, t, easingFunction),
		lerp(a.up, b.up, t, easingFunction),
		lerp(a.direction, b.direction, t, easingFunction)};
}

Camera *Camera::I()
{
	const fvec3 position = fvec3(0, 0, -10);
	const fvec3 target = fvec3(0);
	const fvec3 up = fvec3(0, 1, 0);
	const fvec3 direction = target - position;

	return Camera::I({{position, target, up, direction}});
}

Camera *Camera::I(vector<CameraTransform> states)
{
	if (!Camera::instance.has_value())
	{
		Camera::instance = unique_ptr<Camera>(new Camera(states));
	}
	return Camera::instance.value().get();
}

Camera::Camera(vector<CameraTransform> states) : states(states)
{
	this->transform = states[0];
	this->setProjectionData();

	// Choose cursor mode, and its initial position
	Window::I()->centerCursor();
	Window::I()->disableCursor();
}

void Camera::update(float deltaTime)
{
	if (this->states.size() == 1)
	{
		moveFirstPerson(deltaTime);
		if (!InputEvents::getButtonStates().at(InputEventsType::FREE_CURSOR))
			panFirstPerson();
		return;
	}

	// Interpolate between state[currentState] and state[currentState + 1] based on deltaTime
	const CameraTransform currentState = this->states[this->currentState % this->states.size()];
	const CameraTransform nextState = this->states[(this->currentState + 1) % this->states.size()];

	this->transform = lerp(currentState, nextState, lerpProgress, easeInOutSmoother);

	lerpProgress += deltaTime * lerpSpeed;
	if (lerpProgress >= 1.0f)
	{
		lerpProgress = 0.0f;
		this->currentState++;
	}
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

fvec3 Camera::getPosition() const
{
	return this->transform.position;
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
