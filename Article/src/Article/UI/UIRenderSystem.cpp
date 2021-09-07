#include "pch.h"
#include "UIRenderSystem.h"

#include "UIComponents.h"
#include "Article/Rendering/RenderingComponents.h"
#include "Lithium/Renderer/Renderer.h"

void RenderQuads(entt::registry& registry);
void RenderLabels(entt::registry& registry);

void Systems::UIRender::OnRender(entt::registry& registry)
{
	RenderQuads(registry);
	RenderLabels(registry);
}

void RenderQuads(entt::registry& registry)
{
	Li::Renderer::BeginUI();
	auto ui_transform_view = registry.view<cp::ui_transform>(entt::exclude<cp::label>);
	for (entt::entity entity : ui_transform_view)
	{
		auto& transform = ui_transform_view.get<cp::ui_transform>(entity);

		bool has_color = registry.has<cp::color>(entity);
		bool has_texture = registry.has<cp::texture>(entity);

		std::string texture_alias = has_texture ? registry.get<cp::texture>(entity).alias : "texture_white";
		glm::vec4 color = has_color ? registry.get<cp::color>(entity).color : glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

		Li::Renderer::UISubmitQuad(texture_alias, color, transform.transform, registry.has<cp::ui_texture_crop>(entity));
	}
	Li::Renderer::EndUI();
}

void RenderLabels(entt::registry& registry)
{
	registry.view<cp::ui_transform, cp::label>().each([&registry](entt::entity entity, auto& transform, auto& label)
	{
		glm::vec4 color = registry.has<cp::color>(entity)
			? registry.get<cp::color>(entity).color
			: glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

		Li::Renderer::UISubmitLabel(registry.get<cp::label>(entity).label_ref.get(), transform.transform, color);
	});
}
