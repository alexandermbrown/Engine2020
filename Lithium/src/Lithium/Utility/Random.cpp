#include "lipch.h"
#include "Random.h"

#include "Time.h"
#include <chrono>

namespace Li
{
	Random::Random()
	{
		try {
			std::random_device device;
			m_RNG = std::mt19937(device());
		}
		catch (std::exception e) {
			Li::Log::CoreWarn("Random Number Generator: {}", e.what());

			// Use milliseconds since epoch as random seed instead.
			auto now = std::chrono::steady_clock::now();
			auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
			long long duration = now_ms.time_since_epoch().count();

			m_RNG = std::mt19937(static_cast<unsigned int>(duration));
		}
	}

	float Random::UniformFloat(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(m_RNG);
	}
}
