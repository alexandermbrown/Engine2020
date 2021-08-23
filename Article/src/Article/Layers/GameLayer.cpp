#include "pch.h"
#include "GameLayer.h"

#include "ArticleApp.h"
#include "Article/Scripting/ScriptScene.h"
#include "Article/Gameplay/TransformComponent.h"
#include "Article/Rendering/RenderingSystem.h"
#include "Article/Rendering/RenderingComponents.h"
#include "Article/Gameplay/TransformUpdateSystem.h"
#include "Article/Gameplay/CameraControllerSystem.h"

#include "Article/Gameplay/Player/PlayerComponents.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Resources/ResourceManager.h"

#include "glm/gtc/type_ptr.hpp"

GameLayer::GameLayer()
	: Layer("GameLayer"),
	//m_TerrainStore(11), m_TerrainRenderer(&m_TerrainStore, 3),
	m_Registry()
{
	m_TickThread.Begin(m_Registry);

	CameraControllerSystem::Init(m_Registry);

	//m_TerrainRenderer.LoadTerrain("data/worlds/test.terrain", { 0, 0 });

	m_AudioSource = Li::MakeRef<Li::AudioSource>();
	m_AudioSource->SetAudio(Li::ResourceManager::GetAudioBuffer("audio_wind"));
	m_AudioSource->Play();

	Li::EmitterProps emitter;
	emitter.Count = 256;
	emitter.EmitRate = 10.0f;
	emitter.LifeSpan = glm::vec2{ 1.0f, 2.0f };
	//m_Emitter = Li::MakeRef<Li::ParticleEmitterWI>(emitter);
	m_Emitter = Li::MakeRef<Li::ParticleEmitter>(emitter);
}

GameLayer::~GameLayer()
{
	CameraControllerSystem::Shutdown(m_Registry);
	//m_TerrainRenderer.UnloadTerrain();
	m_TickThread.Finish(m_Registry);
}

void GameLayer::OnUpdate(Li::Duration::us dt)
{
	m_TickThread.UpdateSync(m_Registry, dt);

	auto player_view = m_Registry.view<cp::sync_transform, cp::player>();
	for (entt::entity player : player_view)
	{
		cp::sync_transform& player_transform = player_view.get<cp::sync_transform>(player);

		//m_TerrainRenderer.UpdateCenter({
		//	(int)std::floor(player_transform.position.x / TerrainRenderer::MetersPerChunk),
		//	(int)std::floor(player_transform.position.y / TerrainRenderer::MetersPerChunk)
		//});
		break;
	}

	CameraControllerSystem::Update(m_Registry, dt);

	TransformUpdateSystem::Update(m_Registry);

	cp::camera& camera = m_Registry.ctx<cp::camera>();


	//m_TerrainRenderer.RenderFramebuffer();
	
	Li::Renderer::BeginScene(camera.camera);

	m_Emitter->Update(dt, glm::translate(glm::mat4(1.0f), { 1.0f, 3.0f, 0.0f }));
	m_Emitter->Draw();

	//m_TerrainRenderer.SubmitQuad();
	//RenderingSystem::Render(m_Registry);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_DebugPhysicsRenderer.SubmitLines(m_TickThread.GetDebugDrawQueue());
#endif
	Li::Renderer::EndScene();
}

void GameLayer::OnEvent(SDL_Event* event)
{
	m_TickThread.OnEvent(event);
	CameraControllerSystem::OnEvent(m_Registry, event);

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.scancode)
		{
		case SDL_SCANCODE_F11:
			if (Li::Application::Get().GetWindow().GetFullscreen() == Li::FullscreenType::Windowed) {
				Li::Application::Get().GetWindow().SetFullscreen(Li::FullscreenType::FullscreenWindowed);
			}
			else {
				Li::Application::Get().GetWindow().SetFullscreen(Li::FullscreenType::Windowed);
			}
			break;
		case SDL_SCANCODE_ESCAPE:
			Li::Application::Get().Transition(Li::MakeUnique<ScriptScene>("MainMenuScene"), true);
			break;
		}
	}
}

#ifndef LI_DIST
void GameLayer::OnImGuiRender()
{
}
#endif
