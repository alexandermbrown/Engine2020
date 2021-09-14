#pragma once

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "DebugDrawSystem.h"
#endif

#include "Lithium/Core/Memory.h"
#include "box2d/box2d.h"

namespace cp
{
	struct PhysicsWorld
	{
		Li::Unique<b2World> world;
	};

	struct PhysicsBody
	{
		b2Body* body;
	};

#ifdef HZ_PHYSICS_DEBUG_DRAW
	struct PhysicsDebugDraw
	{
		Li::Unique<Box2DDebugDrawer> draw;
	};
#endif
}
