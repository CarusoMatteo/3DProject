#pragma once

#include "../Model/Transform.h"
#include "EasingFunctions.h"
#include <functional>
#include <vector>

using namespace std;

struct PathState
{
	Transform transform;
	float lerpSpeed = 0.5f;
	function<float(const float)> easingFunction = easeInOutSmoother;
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
