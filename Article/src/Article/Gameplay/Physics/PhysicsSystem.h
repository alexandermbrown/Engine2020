#pragma once

#include "Lithium/Utility/Time.h"
#include "entt/entt.hpp"

namespace PhysicsSystem
{
	void Init(entt::registry& registry);
	void Step(entt::registry& registry, Li::Duration::us dt);
};
