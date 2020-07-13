#pragma once

#include "entt/entt.hpp"
#include "SDL.h"

#include "Horizons/Gameplay/Sync/Sync.h"

class PlayerSystem
{
public:
	static void Init(entt::registry& registry, SyncEventQueue* queue);

	static void Update(entt::registry& registry, float dt);
	static void OnEvent(entt::registry& registry, SDL_Event* event);
};