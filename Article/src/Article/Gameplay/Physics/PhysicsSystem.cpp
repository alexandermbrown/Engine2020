#include "pch.h"
#include "PhysicsSystem.h"

#include "Article/Core/Constants.h"
#include "PhysicsComponents.h"

void PhysicsSystem::Init(entt::registry& registry)
{
	cp::PhysicsWorld& physics_world = registry.set<cp::PhysicsWorld>();
	
	b2Vec2 gravity(0.0f, 0.0f);
	physics_world.world = Li::MakeUnique<b2World>(gravity);
}

void PhysicsSystem::Step(entt::registry& registry, Li::Duration::us dt)
{
	registry.ctx<cp::PhysicsWorld>().world->Step(Li::Duration::fsec(dt).count(),
		Constants::PhysVelocityIterations, Constants::PhysPositionIterations);
}
