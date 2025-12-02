#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Rotation
{
	float angle = 0.0f;
	fvec3 axis = fvec3(0);
};

struct Transform
{
	fvec3 position = fvec3(0);
	Rotation rotation;
	fvec3 scale = fvec3(1);
	fvec3 anchorPoint = fvec3(0);

	fmat4 toMatrix() const
	{
		fmat4 transformMatrix = fmat4(1.0f);
		transformMatrix = translate(transformMatrix, position);
		if (rotation.axis != fvec3(0))
			transformMatrix = rotate(transformMatrix, radians(rotation.angle), rotation.axis);
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
		return glm::lookAt(position, target, up);
	}
};
