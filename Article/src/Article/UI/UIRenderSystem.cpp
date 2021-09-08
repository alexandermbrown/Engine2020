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

		if (const cp::quad* quad = registry.try_get<cp::quad>(entity))
		{
			Li::Renderer::UISubmitQuad(quad->texture_alias, quad->color, transform.transform, registry.has<cp::ui_texture_crop>(entity));
		}
	}
	Li::Renderer::EndUI();
}

void RenderLabels(entt::registry& registry)
{
	registry.view<cp::ui_transform, cp::label>().each([&registry](entt::entity entity, auto& transform, auto& label)
	{
		Li::Renderer::UISubmitLabel(label.label_ref.get(), transform.transform, label.color);
	});
}
