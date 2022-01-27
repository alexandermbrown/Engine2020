#pragma once

#include "entt/entt.hpp"
#include "SDL2/SDL.h"

namespace Systems::UIHover
{
	void OnEvent(entt::registry& registry, SDL_Event* event);
}
