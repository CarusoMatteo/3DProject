#include "../Header Files/Random.h"
#include <random>

random_device Random::rd;
mt19937 Random::gen(Random::rd());

float Random::getRandomFloat(float minIncluded, float maxExcluded)
{
	uniform_real_distribution<float> dis = uniform_real_distribution<float>(minIncluded, maxExcluded);
	return dis(Random::gen);
}

int Random::getRandomInt(int minIncluded, int maxIncluded)
{
	uniform_int_distribution<int> dis = uniform_int_distribution<int>(minIncluded, maxIncluded);
	return dis(Random::gen);
}