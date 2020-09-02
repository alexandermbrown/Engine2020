#include "pch.h"
#include "RNGSystem.h"

#include <random>

void RNGSystem::Init(entt::registry& registry)
{
	std::random_device rd;
	registry.set<std::mt19937>(rd());
}
