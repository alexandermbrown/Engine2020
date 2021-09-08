#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Camera.h"
#include "Lithium/Renderer/Model.h"
#include "Lithium/Renderer/Text/Label.h"
#include "Lithium/Utility/Timer.h"

#include "glm/glm.hpp"

namespace cp
{
	struct quad
	{
		std::string texture_alias;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct model
	{
		Li::Unique<Li::Model> model;
	};

	struct label
	{
		Li::Unique<Li::Label> label_ref;
		glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
	};

	struct camera
	{
		Li::Unique<Li::Camera> camera;
		
		float fov = 70.0f;
		float aspect_ratio = 1.0f;

		float current_zoom = 10.0f;
		float target_zoom = 10.0f;

		float min_zoom = 1.0f;
		float max_zoom = 20.0f;
		float zoom_speed = 5.0f;

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
