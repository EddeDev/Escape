#include "ClientPCH.h"
#include "Random.h"

#include <random>

namespace esc {

	static std::mt19937 s_Engine;
	static std::uniform_real_distribution<float> s_Dist(0.0f, 1.0f);

	float Random::Range(float minInclusive, float maxInclusive)
	{
		return (s_Dist(s_Engine) * (maxInclusive - minInclusive)) + minInclusive;
	}

}