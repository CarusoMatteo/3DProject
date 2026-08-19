#include "../../Header Files/Camera/CameraPath.h"
#include "../../Header Files/Model/Transform.h"
#include <functional>
#include <glm/glm.hpp>
#include <vector>

static fvec3 lerp(const fvec3 a, const fvec3 b, const float t, function<float(const float)> easingFunction)
{
	return a + easingFunction(t) * (b - a);
}

static CameraTransform lerp(const CameraTransform a, const CameraTransform b, const float t, function<float(const float)> easingFunction)
{
	return {
		lerp(a.position, b.position, t, easingFunction),
		lerp(a.target, b.target, t, easingFunction),
		lerp(a.up, b.up, t, easingFunction),
		lerp(a.direction, b.direction, t, easingFunction)};
}

CameraPath::CameraPath(const vector<CameraState> states) : states(states)
{
}

CameraTransform CameraPath::getFirstTransform() const
{
	return this->states[0].transform.toCameraTransform();
}

const CameraTransform CameraPath::getNextTransform(float deltaTime)
{
	// Interpolate between state[currentState] and state[currentState + 1] based on deltaTime
	const CameraState currentState = this->states[this->currentState % this->states.size()];
	const CameraState nextState = this->states[(this->currentState + 1) % this->states.size()];

	const CameraTransform newTransform = lerp(currentState.transform.toCameraTransform(), nextState.transform.toCameraTransform(), lerpProgress, nextState.easingFunction);

	lerpProgress += deltaTime * nextState.lerpSpeed;
	if (lerpProgress >= 1.0f)
	{
		lerpProgress = 0.0f;
		this->currentState++;
	}
	return newTransform;
}
