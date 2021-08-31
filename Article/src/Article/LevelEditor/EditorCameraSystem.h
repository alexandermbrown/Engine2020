#pragma once
#ifndef LI_DIST
#include "Lithium/Utility/Time.h"

#include "SDL.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"

namespace EditorCameraSystem
{
	void Init(entt::registry& registry);
	void Update(entt::registry& registry, Li::Duration::us dt);
	void OnEvent(entt::registry& registry, SDL_Event* event, glm::ivec2 view_size);
	void Resize(entt::registry& registry, int width, int height);
	glm::vec2 GetCameraFocusPoint(entt::registry& registry);
}
#endif
