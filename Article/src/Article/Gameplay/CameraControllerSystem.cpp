#include "pch.h"
#include "CameraControllerSystem.h"

#include "Article/Rendering/RenderingComponents.h"
#include "Article/Gameplay/Player/PlayerComponents.h"
#include "Article/Gameplay/TransformComponent.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Core/Assert.h"

void CameraControllerSystem::Init(entt::registry& registry)
{
	cp::Camera& camera = registry.set<cp::Camera>();

	camera.current_zoom = 10.0f;
	camera.target_zoom = 10.0f;
	camera.aspect_ratio = (float)Li::Application::Get().GetWindow().GetWidth()
		/ (float)Li::Application::Get().GetWindow().GetHeight();

	const float half_zoom = camera.current_zoom * 0.5f;
	camera.camera_ptr = Li::MakeUnique<Li::Camera>();
	camera.camera_ptr->SetPerspective(camera.fov, camera.aspect_ratio, 0.1f, 100.0f);
}

void CameraControllerSystem::Update(entt::registry& registry, Li::Duration::us dt)
{
	cp::Camera& camera = registry.ctx<cp::Camera>();

	for (auto&& [entity, transform, player] : registry.view<cp::Transform, cp::Player>().each())
	{
		glm::vec3 camera_pos = {
			transform.position.x,
			transform.position.y - camera.current_zoom,
			transform.position.z + camera.current_zoom,
		};
		if (camera.camera_ptr->GetPosition() != camera_pos)
		{
			camera.camera_ptr->SetLookAt(camera_pos, transform.position, { 0.0f, 0.0f, 1.0f });
		}
		break;
	}
	
	if (!camera.finished_zoom)
	{
		float delta = camera.current_zoom - camera.target_zoom;
		if (dt < Li::Duration::ms(990))
			camera.current_zoom -= delta * Li::Duration::fsec(dt).count() * camera.zoom_speed;
		else
			camera.current_zoom = camera.target_zoom;

		const float half_zoom = camera.current_zoom * 0.5f;
		camera.camera_ptr->SetPerspective(camera.fov, camera.aspect_ratio, 0.1f, 100.0f);

		if (camera.current_zoom == camera.target_zoom)
			camera.finished_zoom = true;
	}
}

void CameraControllerSystem::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		cp::Camera& camera = registry.ctx<cp::Camera>();

		camera.target_zoom -= event->wheel.y * std::sqrt(camera.target_zoom) / 2.0f;

		if (camera.target_zoom < camera.min_zoom)
			camera.target_zoom = camera.min_zoom;

		else if (camera.target_zoom > camera.max_zoom)
			camera.target_zoom = camera.max_zoom;

		camera.finished_zoom = false;
	}
	else if (event->type == SDL_WINDOWEVENT)
	{
		if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			cp::Camera& camera = registry.ctx<cp::Camera>();

			camera.aspect_ratio = (float)Li::Application::Get().GetWindow().GetWidth()
				/ (float)Li::Application::Get().GetWindow().GetHeight();
			const float half_zoom = camera.current_zoom * 0.5f;
			camera.camera_ptr->SetPerspective(camera.fov, camera.aspect_ratio, 0.1f, 100.0f);
		}
	}
}
