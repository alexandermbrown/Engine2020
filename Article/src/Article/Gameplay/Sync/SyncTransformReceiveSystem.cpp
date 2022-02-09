#include "pch.h"
#include "SyncTransformReceiveSystem.h"

#include "Article/Gameplay/TransformComponent.h"
#include "Article/Gameplay/Sync/Sync.h"

void AddSyncTransform(entt::registry& registry, entt::entity entity)
{
	cp::SyncTransform& sync_transform = registry.get<cp::SyncTransform>(entity);
	cp::Transform& transform = registry.emplace<cp::Transform>(entity);

	transform.position = sync_transform.position;
	transform.rotation = sync_transform.rotation;

	transform.old = true;
}

void SyncTransformReceiveSystem::Init(entt::registry& registry)
{
	registry.on_construct<cp::SyncTransform>().connect<&AddSyncTransform>();
}

void SyncTransformReceiveSystem::Update(entt::registry& registry, SyncTransformQueue* queue, Li::Duration::us dt)
{
	// Possible Improvement: Include a timestamp in the queue to better synchronize.

	cp::SyncTransform received;
	while (queue->try_dequeue(received))
	{
		entt::entity entity = registry.ctx<cp::SyncTracker>().map.at(received.sync_id);
		registry.emplace_or_replace<cp::SyncTransform>(entity, received);

		cp::Transform& transform = registry.get_or_emplace<cp::Transform>(entity);
		transform.position = { received.position.x, received.position.y, received.position.z };
		transform.rotation = received.rotation;

		transform.old = true;
	}

	float dt_float = Li::Duration::fsec(dt).count();
	registry.view<cp::SyncTransform, cp::Transform>().each([dt_float](cp::SyncTransform& sync, cp::Transform& transform)
	{
		if (sync.velocity.x || sync.velocity.y || sync.velocity.z || sync.angular_velocity != glm::identity<glm::quat>())
		{
			transform.position.x += sync.velocity.x * dt_float;
			transform.position.y += sync.velocity.y * dt_float;
			transform.position.z += sync.velocity.z * dt_float;
			transform.rotation;
			
			glm::quat rotation = glm::mix(glm::identity<glm::quat>(), sync.angular_velocity, dt_float);
			transform.rotation = rotation * transform.rotation;

			transform.old = true;
		}
	});
}