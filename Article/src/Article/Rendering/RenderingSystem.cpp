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

		std::string alias = registry.has<cp::texture>(entity)
			? registry.get<cp::texture>(entity).alias
			: "texture_white";

		glm::vec4 color = registry.has<cp::color>(entity)
			? registry.get<cp::color>(entity).color
			: glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

		Li::Renderer::SubmitQuad(alias, color, transform.transform);
	}
}
