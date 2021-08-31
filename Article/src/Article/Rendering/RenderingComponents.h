#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Camera.h"
#include "Lithium/Renderer/Text/Label.h"
#include "Lithium/Utility/Timer.h"

#include "glm/glm.hpp"

namespace cp
{
	struct texture
	{
		std::string alias;
	};

	struct color
	{
		glm::vec4 color;
	};

	struct label
	{
		Li::Ref<Li::Label> label_ref;
	};

	struct camera
	{
		Li::Unique<Li::Camera> camera;
		
		float aspect_ratio = 1.0f;

		float current_zoom = 10.0f;
		float target_zoom = 10.0f;

		float min_zoom = 2.0f;
		float max_zoom = 100.0f;
		float zoom_speed = 10.0f;

		bool finished_zoom = false;
	};

	struct flicker
	{
		float magnitude = 0.1f;
		Li::Timer timer;
		float current;
		float target;
	};
}
