#include "pch.h"
#include "PlayerSystem.h"

#include "PlayerComponents.h"
#include "Article/Gameplay/Physics/PhysicsComponents.h"
#include "Article/Rendering/RenderingComponents.h"
#include "Article/Gameplay/TransformComponent.h"

#include "Article/Core/Game.h"

void PlayerSystem::Init(entt::registry& registry, SyncEventQueue* queue)
{
	entt::entity player = registry.create();
	registry.emplace<cp::Player>(player);

	// Sync player with rendering thread.
	cp::SyncManager& sync_manager = registry.ctx<cp::SyncManager>();

	uint64_t sync_id = sync_manager.count;
	sync_manager.count++;

	registry.emplace<cp::Sync>(player, sync_id);
	registry.emplace<cp::SyncTransform>(player, sync_id, glm::vec3{ 5.0f, 1.0f, 0.0f }  );

	cp::SyncTransform* transform = new cp::SyncTransform{ sync_id, { 5.0f, 1.0f, 0.0f } };

	queue->enqueue(SyncEvent::CreateEntity(sync_id));
	queue->enqueue(SyncEvent::AddComponent<cp::Player>(sync_id, nullptr));
	queue->enqueue(SyncEvent::AddComponent<cp::SyncTransform>(sync_id, transform));

	// Init physics body.
	cp::PhysicsBody& phys_body = registry.emplace<cp::PhysicsBody>(player);

	// Get physics world from the registry.
	cp::PhysicsWorld& world = registry.ctx<cp::PhysicsWorld>();

	// Create the player's physics body.
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position.Set(5.0f, 1.0f); // TODO: define where the player is in the world.
	phys_body.body = world.world->CreateBody(&player_body_def);

		
	b2CircleShape player_shape;
	player_shape.m_p.Set(0.0f, 0.0f);
	player_shape.m_radius = 0.5f;

	b2FixtureDef fixture_def;
	fixture_def.shape = &player_shape;
	fixture_def.density = 1.0f; // TODO: Set player density.
	fixture_def.friction = 0.3f; // TODO: Set player friction.
	fixture_def.restitution = 0.0f; // TODO: Set player restitution.

	phys_body.body->CreateFixture(&fixture_def);
}

void PlayerSystem::Update(entt::registry& registry, Li::Duration::us dt)
{
	auto view = registry.view<cp::Player, cp::PhysicsBody>();
	cp::Player& player = view.get<cp::Player>(view.front());
	cp::PhysicsBody& body = view.get<cp::PhysicsBody>(view.front());

	player.move_direction = {
		(player.right ? 1.0f : 0.0f) - (player.left ? 1.0f : 0.0f),
		(player.up ? 1.0f : 0.0f) - (player.down ? 1.0f : 0.0f)
	};

	// Ensure the move direction is of length 1.
	float length = glm::length(player.move_direction);
	if (length != 0.0f)
		player.move_direction /= length;

	// Apply an impulse to the physics body to get instant movement.
	b2Vec2 vel = body.body->GetLinearVelocity();
	b2Vec2 desired_vel = { player.move_direction.x * player.move_speed, player.move_direction.y * player.move_speed };
	b2Vec2 delta_vel = desired_vel - vel;

	// Only apply if changed.
	if (delta_vel != b2Vec2_zero)
		body.body->ApplyLinearImpulse(body.body->GetMass() * delta_vel, body.body->GetWorldCenter(), true);
}

void PlayerSystem::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		ConfigStore& config = Game::Get()->GetConfig();

		if (event->key.keysym.scancode == config.Get<int>("keybind_move_left"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.left = true;
		}
		else if (event->key.keysym.scancode == config.Get<int>("keybind_move_right"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.right = true;
		}
		else if (event->key.keysym.scancode == config.Get<int>("keybind_move_up"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.up = true;
		}
		else if (event->key.keysym.scancode == config.Get<int>("keybind_move_down"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.down = true;
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		ConfigStore& config = Game::Get()->GetConfig();

		if (event->key.keysym.scancode == config.Get<int>("keybind_move_left"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.left = false;
		}
		else if (event->key.keysym.scancode == config.Get<int>("keybind_move_right"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.right = false;
		}
		else if (event->key.keysym.scancode == config.Get<int>("keybind_move_up"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.up = false;
		}
		else if (event->key.keysym.scancode == config.Get<int>("keybind_move_down"))
		{
			auto view = registry.view<cp::Player>();
			cp::Player& player = view.get<cp::Player>(view.front());

			player.down = false;
		}
	}
}
