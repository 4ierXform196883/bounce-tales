#include "randomizer.hpp"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_real_distribution<> dist(0, 1);

int randomizer::getRandInt(int start, int finish)
{
    return start + dist(rng) * (finish - start);
}

float randomizer::getRandFloat(float start, float finish)
{
    return start + dist(rng) * (finish - start);
}
