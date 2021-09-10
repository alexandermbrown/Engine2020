#include "pch.h"
#include "FlickerSystem.h"

#include "RenderingComponents.h"

#include "Lithium/Utility/Random.h"

void Systems::Flicker::OnUpdate(entt::registry& registry, Li::Duration::us dt)
{
	Li::Random& rand = registry.ctx<Li::Random>();

	registry.view<cp::Quad, cp::flicker>().each([dt, &rand](entt::entity entity, cp::Quad& quad, cp::flicker& flicker)
	{
		float value = flicker.current + (flicker.target - flicker.current) * flicker.timer.GetFraction();
		quad.color.r = value;
		quad.color.g = value;
		quad.color.b = value;
		if (flicker.timer.Update(dt))
		{
			flicker.current = value;
			flicker.target = rand.UniformFloat(1.0f - flicker.magnitude, 1.0f);
		}
	});
}
