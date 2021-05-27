#pragma once

#include "Lithium/Utility/Time.h"
#include "entt/entt.hpp"

namespace Systems::UISortTransforms
{
	void OnUpdate(entt::registry& registry, Li::Duration::us dt);
}
