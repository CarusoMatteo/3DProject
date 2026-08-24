#pragma once

#include <cmath>

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
