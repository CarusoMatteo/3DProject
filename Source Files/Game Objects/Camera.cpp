#include "../../Header Files/Game Objects/Camera.h"
#include "../../Header Files/Window.h"

Camera::Camera()
{
	this->projectionData = Window::I()->makeProjectionData();
}

void Camera::update(float deltaTime)
{
	// TODO
}

void Camera::render()
{
	return;
}
