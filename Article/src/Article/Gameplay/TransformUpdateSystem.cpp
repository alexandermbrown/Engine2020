#include "pch.h"
#include "TransformUpdateSystem.h"

#include "TransformComponent.h"
#include "glm/gtc/matrix_transform.hpp"

void TransformUpdateSystem::Update(entt::registry& registry)
{
	registry.view<cp::Transform>().each([](cp::Transform& transform) {
		if (transform.old)
		{
			transform.transform = glm::translate(glm::mat4(1.0f), transform.position);

			if (transform.rotation != glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f })
				transform.transform *= glm::toMat4(transform.rotation);

			transform.transform *= glm::scale(glm::mat4(1.0f), transform.scale);

			transform.old = false;
		}
	});
}
