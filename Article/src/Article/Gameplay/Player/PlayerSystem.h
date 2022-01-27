#pragma once

#include "Article/Gameplay/Sync/Sync.h"

#include "Lithium/Utility/Time.h"
#include "entt/entt.hpp"
#include "SDL2/SDL.h"

namespace PlayerSystem
{
	void Init(entt::registry& registry, SyncEventQueue* queue);

	void Update(entt::registry& registry, Li::Duration::us dt);
	void OnEvent(entt::registry& registry, SDL_Event* event);
};
