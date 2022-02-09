#include "pch.h"
#include "SyncEventReceiveSystem.h"

#include "Article/Gameplay/TransformComponent.h"
#include "Article/Gameplay/Player/PlayerComponents.h"
#include "Article/Rendering/RenderingComponents.h"
#include "Article/Gameplay/Sync/SyncTransform.h"

#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Log.h"

template<typename Component>
void AddComponent(entt::registry& registry, SyncEvent& event)
{
	cp::SyncTracker& tracker = registry.ctx<cp::SyncTracker>();
	entt::entity entity = tracker.map[event.sync_id];
	LI_ASSERT(registry.get<cp::Sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

	if (event.data)
	{
		Component* recieved_cp = static_cast<Component*>(event.data);
		registry.emplace<Component>(entity, *recieved_cp);
		delete recieved_cp;
	}
	else
	{
		registry.emplace<Component>(entity);
	}
}

template<typename Component>
void RemoveComponent(entt::registry& registry, SyncEvent& event)
{
	cp::SyncTracker& tracker = registry.ctx<cp::SyncTracker>();
	entt::entity entity = tracker.map[event.sync_id];
	LI_ASSERT(registry.get<cp::Sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

	registry.remove<Component>(entity);
}

void SyncEventReceiveSystem::Init(entt::registry& registry)
{
	cp::SyncTracker& tracker = registry.set<cp::SyncTracker>();
	tracker.map = std::unordered_map<uint64_t, entt::entity>();
}

void SyncEventReceiveSystem::Update(entt::registry& registry, SyncEventQueue* queue)
{
	SyncEvent event;
	while (queue->try_dequeue(event))
	{
		if (event.type == SyncType::CreateEntity)
		{
			entt::entity new_ent = registry.create();
			registry.emplace<cp::Sync>(new_ent, event.sync_id);

			registry.ctx<cp::SyncTracker>().map[event.sync_id] = new_ent;

		}
		else if (event.type == SyncType::RemoveEntity)
		{
			cp::SyncTracker& tracker = registry.ctx<cp::SyncTracker>();
			entt::entity entity = tracker.map[event.sync_id];
			LI_ASSERT(registry.get<cp::Sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

			registry.destroy(entity);
			tracker.map.erase(event.sync_id);
		}
		else if (event.type == SyncType::AddComponent)
		{
			if (event.data1 == entt::type_index<cp::SyncTransform>::value())
				AddComponent<cp::SyncTransform>(registry, event);

			else if (event.data1 == entt::type_index<cp::Transform>::value())
				AddComponent<cp::Transform>(registry, event);

			else if (event.data1 == entt::type_index<cp::Quad>::value())
				AddComponent<cp::Quad>(registry, event);

			else if (event.data1 == entt::type_index<cp::Player>::value())
				AddComponent<cp::Player>(registry, event);

			else Li::Log::Error("Unknown component type {}", event.data1);
		}
		else if (event.type == SyncType::RemoveComponent)
		{
			if (event.data1 == entt::type_index<cp::SyncTransform>::value())
				RemoveComponent<cp::SyncTransform>(registry, event);

			else if (event.data1 == entt::type_index<cp::Transform>::value())
				RemoveComponent<cp::Transform>(registry, event);

			else if (event.data1 == entt::type_index<cp::Quad>::value())
				RemoveComponent<cp::Quad>(registry, event);

			else if (event.data1 == entt::type_index<cp::Player>::value())
				RemoveComponent<cp::Player>(registry, event);

			else Li::Log::Error("Unknown component type {}", event.data1);
		}
		else
		{
			Li::Log::Error("Unknown sync event type!");
		}
	}
}
