#pragma once

#include "entt/entt.hpp"
#include "SDL2/SDL.h"

namespace Systems::UIClick
{
	void OnEvent(entt::registry& registry, SDL_Event* event);
}
