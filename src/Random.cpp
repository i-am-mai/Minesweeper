#include "Random.h"
#include <ctime>

std::mt19937 Random::random(time(0));

int Random::Integer(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}

float Random::Float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random);
}

// Takes a random sample of numVals from min (INCLUSIVE) to max (EXCLUSIVE)
void Random::RandomSample(int min, int max, int numVals, std::vector<int>& vals)
{
	// Algorithm R by Alan Waterman for random sampling.
	for (int i = min; i < (min + numVals); i++)
		vals.push_back(i);

	for (int i = numVals; i < max; i++)
	{
		int j = Integer(0, i);
		if (j < numVals)
			vals[j] = i;
	}
}