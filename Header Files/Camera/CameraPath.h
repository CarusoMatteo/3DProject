#pragma once

#include "../Model/Transform.h"
#include "EasingFunctions.h"
#include <functional>
#include <vector>

using namespace std;

struct SimpleCameraTransForm
{
	fvec3 position;
	fvec3 target;
	fvec3 up;

	CameraTransform toCameraTransform() const
	{
		return CameraTransform{position, target, up, target - position};
	}
};

struct CameraState
{
	SimpleCameraTransForm transform;
	float lerpSpeed = 0.5f;
	function<float(const float)> easingFunction = easeInOutSmoother;
};

class CameraPath
{
public:
	CameraPath(const vector<CameraState> states);
	~CameraPath() = default;

	CameraTransform getFirstTransform() const;
	const CameraTransform getNextTransform(float deltaTime);

private:
	vector<CameraState> states;
	unsigned int currentState = 0;
	float lerpProgress = 0;
};
