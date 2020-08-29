﻿#include "pch.h"
#include "GameLayer.h"

#include "Horizons.h"
#include "Horizons/Core/TickThread.h"
#include "Horizons/Gameplay/Components.h"
#include "Horizons/Terrain/TerrainManager.h"
#include "Horizons/Rendering/RenderingSystem.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Gameplay/TransformUpdateSystem.h"
#include "Horizons/Gameplay/CameraControllerSystem.h"
#include "Horizons/Gameplay/Sync/SyncEventReceiveSystem.h"
#include "Horizons/Gameplay/Sync/SyncTransformReceiveSystem.h"

#include "Horizons/Gameplay/Player/PlayerComponents.h"
#include "Horizons/Gameplay/Sync/SyncTransform.h"
#include "Horizons/Gameplay/Components.h"

#include "Horizons/Terrain/SimplexNoise.h"

#include "entt/entt.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <thread>

GameLayer::GameLayer()
	: Layer("GameLayer"), m_EventQueue(256ull), m_SyncQueue(256ull), m_TransformQueue(256ull), m_Registry()
#ifdef HZ_PHYSICS_DEBUG_DRAW
	, m_DebugDrawQueue(256ull), m_DebugPhysicsRenderer(&m_DebugDrawQueue)
#endif
{
	// Start up tick thread.
	TickThreadData threadData;
	threadData.EventQueue = &m_EventQueue;
	threadData.SyncQueue = &m_SyncQueue;
	threadData.TransformQueue = &m_TransformQueue;
	threadData.Config = li::Application::Get<Horizons>()->GetConfig();

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_TickThread = std::thread(TickThreadEntryPointDebugDraw, threadData, &m_DebugDrawQueue);
#else
	m_TickThread = std::thread(TickThreadEntryPoint, threadData);
#endif

	li::Renderer::AddTextureAtlas(li::ResourceManager::Get<li::TextureAtlas>("atlas_test"));

	entt::entity test = m_Registry.create();
	m_Registry.emplace<cp::texture>(test, "test_small");
	cp::transform& transform = m_Registry.emplace<cp::transform>(test);

	transform.position = { 1.0f, -1.0f, 0.4f };
	transform.rotation = 1.0f;
	transform.scale = { 1.0f, 2.0f, 1.0f };
	transform.old = true;

	SyncEventReceiveSystem::Init(m_Registry);
	SyncTransformReceiveSystem::Init(m_Registry);
	CameraControllerSystem::Init(m_Registry);

	TerrainManager::LoadWorld("data/worlds/test.terrain", { 0, 0 });
}

GameLayer::~GameLayer()
{
	CameraControllerSystem::Shutdown(m_Registry);
}

void GameLayer::OnAttach()
{
	m_AudioSource = li::CreateRef<li::AudioSource>();
	m_AudioSource->SetAudio(li::ResourceManager::Get<li::Audio>("audio_wind"));
	m_AudioSource->Play();
}

void GameLayer::OnDetach()
{
	TerrainManager::UnloadWorld();
	m_TickThread.join();
	// Clear queue to free pointers.
	SyncEventReceiveSystem::Update(m_Registry, &m_SyncQueue);
}

void GameLayer::OnUpdate(float dt)
{
	SyncEventReceiveSystem::Update(m_Registry, &m_SyncQueue);
	SyncTransformReceiveSystem::Update(m_Registry, &m_TransformQueue, dt);

	auto player_view = m_Registry.view<cp::sync_transform, cp::player>();
	for (entt::entity player : player_view)
	{
		cp::sync_transform& player_transform = player_view.get<cp::sync_transform>(player);

		TerrainManager::UpdateCenter({
			(int)std::floor(player_transform.position.x / TerrainManager::MetersPerChunk),
			(int)std::floor(player_transform.position.y / TerrainManager::MetersPerChunk)
		});
		break;
	}

	CameraControllerSystem::Update(m_Registry, dt);

	TransformUpdateSystem::Update(m_Registry);


	cp::camera& camera = m_Registry.ctx<cp::camera>();

	TerrainManager::RenderFramebuffer();

	li::RendererAPI::BindDefaultRenderTarget();
	li::RendererAPI::Clear();
	li::Renderer::BeginScene(camera.camera);

	TerrainManager::RenderQuad();
	RenderingSystem::Render(m_Registry);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	//m_DebugPhysicsRenderer.Render();
#endif

	li::Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(SDL_Event* event)
{
	m_EventQueue.enqueue(*event);

	CameraControllerSystem::OnEvent(m_Registry, event);

	li::EventDispatcher dispatcher(event);
	dispatcher.Dispatch(SDL_KEYUP, [&](SDL_Event* e) {

		if (e->key.keysym.scancode == SDL_SCANCODE_F11) {
			if (li::Application::Get()->GetWindow()->GetFullscreen() == li::FullscreenType::Windowed) {
				li::Application::Get()->GetWindow()->SetFullscreen(li::FullscreenType::FullscreenWindowed);
			}
			else {
				li::Application::Get()->GetWindow()->SetFullscreen(li::FullscreenType::Windowed);
			}
		}
	});
}
