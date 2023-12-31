#pragma once

#include "glm/glm.hpp"

namespace cp
{
	struct Player
	{
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;

		float move_speed = 8.0f;
		glm::vec2 move_direction = { 0.0f, 0.0f };
	};
}
