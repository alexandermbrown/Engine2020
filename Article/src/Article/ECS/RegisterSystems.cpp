#include "pch.h"
#include "RegisterSystems.h"

#include "Article/Gameplay/RNGSystem.h"

#include "Article/UI/UIClickSystem.h"
#include "Article/UI/UIHoverSystem.h"
#include "Article/UI/UILayoutSystem.h"
#include "Article/UI/UITransformSortSystem.h"
#include "Article/UI/UIRenderSystem.h"

#include "Article/Rendering/FlickerSystem.h"

void RegisterSystems(SystemRegistry& registry)
{
	// Gameplay //
	registry.Register("RNG")
		.SetOnInit(Systems::RNG::OnInit);

	// Rendering //
	registry.Register("Flicker")
		.SetOnUpdate(Systems::Flicker::OnUpdate);

	// UI //
	registry.Register("UIClick")
		.SetOnEvent(Systems::UIClick::OnEvent);

	registry.Register("UIHover")
		.SetOnEvent(Systems::UIHover::OnEvent);

	registry.Register("UILayout")
		.SetOnEvent(Systems::UILayout::OnEvent)
		.SetOnUpdate(Systems::UILayout::OnUpdate)
		.SetOnDeinit(Systems::UILayout::OnDeinit);

	registry.Register("UISortTransforms")
		.SetOnUpdate(Systems::UISortTransforms::OnUpdate);
	
	registry.Register("UIRender")
		.SetOnRender(Systems::UIRender::OnRender);
}
