#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Transform
{
	fvec3 position = fvec3(0);
	float rotation = 0.0f;
	fvec3 rotationAxis = fvec3(0);
	fvec3 scale = fvec3(1);
	fvec3 anchorPoint = fvec3(0);

	fmat4 toMatrix() const
	{
		fmat4 transformMatrix = fmat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, position);
		transformMatrix = glm::rotate(transformMatrix, rotation, rotationAxis);
		transformMatrix = glm::scale(transformMatrix, scale);
		return transformMatrix;
	}
};

struct CameraTransform
{
	fvec3 position;
	fvec3 target;
	fvec3 up;
	fvec3 direction;

	fmat4 toMatrix() const
	{
		// TODO: May be wrong
		return glm::lookAt(position, target, up);
	}
};
