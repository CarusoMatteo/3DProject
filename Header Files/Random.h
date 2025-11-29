#pragma once

#include <random>

using namespace std;

/// @brief Static class that provides random number generation utilities.
class Random
{
public:
	/// @brief Get a random float in the range [minIncluded, maxExcluded).
	static float getRandomFloat(const float minIncluded, const float maxExcluded);
	/// @brief Get a random int in the range [minIncluded, maxIncluded].
	static int getRandomInt(const int minIncluded, const int maxIncluded);

	static random_device rd;
	static mt19937 gen;

private:
	Random() = default;
	~Random() = default;
};