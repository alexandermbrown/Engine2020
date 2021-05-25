#include "pch.h"
#include "RNGSystem.h"

#include "Lithium.h"

void Systems::RNG::OnInit(entt::registry& registry)
{
	registry.set<Li::Random>();
}
