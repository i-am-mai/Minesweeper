#pragma once
#include <random>
#include <vector>

class Random
{
	static std::mt19937 random;

public:
	static int Integer(int min, int max);
	static float Float(float min, float max);
	static void RandomSample(int min, int max, int numVals, std::vector<int>& vals);
};