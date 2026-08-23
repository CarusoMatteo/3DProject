#include "../../Header Files/Camera/CameraPath.h"
#include "../../Header Files/Model/Transform.h"
#include <functional>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

static float lerp(const float a, const float b, const float t, function<float(const float)> easingFunction)
{
	return a + easingFunction(t) * (b - a);
}

static fvec3 lerp(const fvec3 a, const fvec3 b, const float t, function<float(const float)> easingFunction)
{
	return a + easingFunction(t) * (b - a);
}

static CameraFullTransform lerp(const CameraFullTransform a, const CameraFullTransform b, const float t, function<float(const float)> easingFunction)
{
	return {
		lerp(a.position, b.position, t, easingFunction),
		lerp(a.target, b.target, t, easingFunction),
		lerp(a.up, b.up, t, easingFunction),
		lerp(a.direction, b.direction, t, easingFunction)};
}

static Transform lerp(const Transform a, const Transform b, const float t, function<float(const float)> easingFunction)
{
	return {
		lerp(a.position, b.position, t, easingFunction),
		Rotation{
			lerp(a.rotation.angle, b.rotation.angle, t, easingFunction),
			lerp(a.rotation.axis, b.rotation.axis, t, easingFunction)},
		lerp(a.scale, b.scale, t, easingFunction)};
}

CameraPath::CameraPath(const vector<CameraPathState> states) : states(states)
{
}

CameraFullTransform CameraPath::getFirstTransform() const
{
	return this->states[0].transform.toCameraTransform();
}

const CameraFullTransform CameraPath::getNextTransform(float deltaTime)
{
	// Interpolate between state[currentState] and state[currentState + 1] based on deltaTime
	const CameraPathState currentState = this->states[this->currentState % this->states.size()];
	const CameraPathState nextState = this->states[(this->currentState + 1) % this->states.size()];

	const CameraFullTransform newTransform = lerp(currentState.transform.toCameraTransform(), nextState.transform.toCameraTransform(), lerpProgress, nextState.easingFunction);

	lerpProgress += deltaTime * nextState.lerpSpeed;
	if (lerpProgress >= 1.0f)
	{
		lerpProgress = 0.0f;
		this->currentState++;
	}
	return newTransform;
}

Path::Path(const vector<PathState> states) : states(states)
{
}

Transform Path::getFirstTransform() const
{
	return this->states[0].transform;
}

const Transform Path::getNextTransform(float deltaTime)
{
	// Interpolate between state[currentState] and state[currentState + 1] based on deltaTime
	const PathState currentState = this->states[this->currentState % this->states.size()];
	const PathState nextState = this->states[(this->currentState + 1) % this->states.size()];

	const Transform newTransform = lerp(currentState.transform, nextState.transform, lerpProgress, nextState.easingFunction);

	lerpProgress += deltaTime * nextState.lerpSpeed;
	if (lerpProgress >= 1.0f)
	{
		lerpProgress = 0.0f;
		this->currentState++;
	}
	return newTransform;
}
