#pragma once

#include "../Model/Transform.h"
#include "EasingFunctions.h"
#include <functional>
#include <vector>

using namespace std;

struct CameraTransform
{
	fvec3 position;
	fvec3 target;
	fvec3 up = fvec3(0, 1, 0);

	CameraFullTransform toCameraTransform() const
	{
		return CameraFullTransform{position, target, up, target - position};
	}
};

struct PathState
{
	Transform transform;
	float lerpSpeed = 0.5f;
	function<float(const float)> easingFunction = easeInOutSmoother;
};

struct CameraPathState
{
	CameraTransform transform;
	float lerpSpeed = 0.5f;
	function<float(const float)> easingFunction = easeInOutSmoother;
};

class CameraPath
{
public:
	CameraPath(const vector<CameraPathState> states);
	~CameraPath() = default;

	CameraFullTransform getFirstTransform() const;
	const CameraFullTransform getNextTransform(float deltaTime);

private:
	vector<CameraPathState> states;
	unsigned int currentState = 0;
	float lerpProgress = 0;
};

class Path
{
public:
	Path(const vector<PathState> states);
	~Path() = default;

	Transform getFirstTransform() const;
	const Transform getNextTransform(float deltaTime);

private:
	vector<PathState> states;
	unsigned int currentState = 0;
	float lerpProgress = 0;
};
