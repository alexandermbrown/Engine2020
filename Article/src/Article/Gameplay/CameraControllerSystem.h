#pragma once

#include "Lithium/Utility/Time.h"
#include "entt/entt.hpp"
#include "SDL.h"

namespace CameraControllerSystem
{
	void Init(entt::registry& registry);
	void Update(entt::registry& registry, Li::Duration::us dt);
	void OnEvent(entt::registry& registry, SDL_Event* event);
};
