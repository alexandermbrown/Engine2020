#include "pch.h"
#include "RenderingSystem.h"

#include "RenderingComponents.h"
#include "Article/Gameplay/TransformComponent.h"

#include "Lithium/Renderer/Renderer.h"

void RenderingSystem::Render(entt::registry& registry)
{
	registry.sort<cp::transform>([](const cp::transform& lhs, const cp::transform& rhs) {
		return lhs.position.z < rhs.position.z;
	});

	auto transform_view = registry.view<cp::transform>();
	for (entt::entity entity : transform_view)
	{
		const auto& transform = transform_view.get<cp::transform>(entity);

		if (const cp::quad* quad = registry.try_get<cp::quad>(entity))
		{
			Li::Renderer::SubmitQuad(quad->texture_alias, quad->color, transform.transform);
		}
	}
}
