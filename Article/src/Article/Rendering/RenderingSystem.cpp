#include "pch.h"
#include "RenderingSystem.h"

#include "RenderingComponents.h"
#include "Article/Gameplay/TransformComponent.h"

#include "Lithium/Renderer/Renderer.h"

void RenderingSystem::Render(entt::registry& registry)
{
	cp::Camera& camera = registry.ctx<cp::Camera>();

	registry.sort<cp::Transform>([&camera](const cp::Transform& lhs, const cp::Transform& rhs) {

		const glm::vec3& camera_pos = camera.camera_ptr->GetPosition();
		float distance_lhs = glm::distance2(camera_pos, lhs.position);
		float distance_rhs = glm::distance2(camera_pos, rhs.position);

		return distance_lhs > distance_rhs;
		//return lhs.position.z < rhs.position.z;
	});

	auto transform_view = registry.view<cp::Transform>();
	for (entt::entity entity : transform_view)
	{
		const auto& transform = transform_view.get<cp::Transform>(entity);

		if (const cp::Quad* quad = registry.try_get<cp::Quad>(entity))
			Li::Renderer::SubmitQuad(quad->texture_alias, quad->color, transform.transform);

		if (const cp::Model* model = registry.try_get<cp::Model>(entity))
			Li::Renderer::SubmitModel(model->model.get(), transform.transform);
	}
}
