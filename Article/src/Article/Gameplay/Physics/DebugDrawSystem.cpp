#include "pch.h"
#include "DebugDrawSystem.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "PhysicsComponents.h"

void DebugDrawSystem::Init(entt::registry& registry, DebugDrawCommandQueue* queue)
{
	cp::PhysicsWorld& world = registry.ctx<cp::PhysicsWorld>();

	cp::PhysicsDebugDraw& draw = registry.set<cp::PhysicsDebugDraw>();
	draw.draw = Li::MakeUnique<Box2DDebugDrawer>(queue);
	draw.draw->SetFlags(0b01011);
	world.world->SetDebugDraw(draw.draw.get());
}

void DebugDrawSystem::Draw(entt::registry& registry)
{
	cp::PhysicsWorld& world = registry.ctx<cp::PhysicsWorld>();

	cp::PhysicsDebugDraw& debug_draw = registry.ctx<cp::PhysicsDebugDraw>();

	world.world->DebugDraw();
	debug_draw.draw->EndDraw();
}


Box2DDebugDrawer::Box2DDebugDrawer(DebugDrawCommandQueue* queue)
	: m_CommandQueue(queue) {}

void Box2DDebugDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertex_count, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::PolygonOutline;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Vertices = new glm::vec2[vertex_count];
	command.VertexCount = vertex_count;

	for (int i = 0; i < vertex_count; i++)
	{
		command.Vertices[i] = { vertices[i].x, vertices[i].y };
	}

	m_CommandQueue->enqueue(command);
}

void Box2DDebugDrawer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::Circle;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Point1 = { center.x, center.y };
	command.Radius = radius;

	m_CommandQueue->enqueue(command);
}

void Box2DDebugDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::Line;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Point1 = { p1.x, p1.y };
	command.Point2 = { p2.x, p2.y };

	m_CommandQueue->enqueue(command);
}

void Box2DDebugDrawer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::Point;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Point1 = { p.x, p.y };
	command.Radius = size;

	m_CommandQueue->enqueue(command);
}

void Box2DDebugDrawer::EndDraw()
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::End;
	m_CommandQueue->enqueue(command);
}
#endif
