#include "pch.h"
#include "SyncTransformSendSystem.h"

#include "Article/Gameplay/Physics/PhysicsComponents.h"

void SyncTransformSendSystem::Update(entt::registry& registry, SyncTransformQueue* queue)
{
	registry.view<cp::SyncTransform, cp::PhysicsBody>().each([queue](cp::SyncTransform& sync_transform, cp::PhysicsBody& body)
	{
		cp::SyncTransform transform;

		transform.position = { body.body->GetPosition().x, body.body->GetPosition().y, sync_transform.position.z };
		transform.velocity = { body.body->GetLinearVelocity().x, body.body->GetLinearVelocity().y, 0.0f };
		
		transform.rotation = glm::quat(glm::vec3{ 0.0f, 0.0f, body.body->GetAngle() });
		transform.angular_velocity = glm::quat(glm::vec3{ 0.0f, 0.0f, body.body->GetAngularVelocity() });

		if (transform != sync_transform)
		{
			sync_transform = transform;
			queue->enqueue(sync_transform);
		}
	});
}
