#pragma once

#include <random>

namespace Li
{
	class Random
	{
	public:
		Random();
		float UniformFloat(float min, float max);

	private:
		std::mt19937 m_RNG;
	};
}
