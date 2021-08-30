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

#include <chrono>
using namespace std::chrono_literals;

GameLayer::GameLayer()
	: Layer("GameLayer"),
	//m_TerrainStore(11), m_TerrainRenderer(&m_TerrainStore, 3),
	m_BurstTimer(1s, false, true)
{
	m_TickThread.Begin(m_Registry);

	CameraControllerSystem::Init(m_Registry);

	//m_TerrainRenderer.LoadTerrain("data/worlds/test.terrain", { 0, 0 });

	m_AudioSource = Li::MakeRef<Li::AudioSource>();
	m_AudioSource->SetAudio(Li::ResourceManager::GetAudioBuffer("audio_wind"));
	m_AudioSource->Play();

	Li::EmitterProps emitter;
	emitter.MaxCount = 256;
	emitter.Continuous = false;
	emitter.RelativeToWorld = true;
	emitter.LifeSpan = { 2.0f, 3.0f };
	emitter.SpeedRange = { 0.0f, 0.5f };
	emitter.EmitVolume = { 0.2f, 0.3f, 0.0f };
	emitter.EmitRate = 4000.0f;
	//emitter.EmitRate = emitter.MaxCount / (emitter.LifeSpan.y - (emitter.LifeSpan.y - emitter.LifeSpan.x) / 2.1f);
	emitter.ParticleScale = { 0.2f, 0.2f, 1.0f };

	emitter.InitialAngle = { 0.0f, (float)M_PI / 4.0f };
	emitter.AngularVelocity = { -1.0f, 1.0f };

	emitter.Acceleration = { 0.0f, 3.0f, 0.0f };

	emitter.ScaleGraph[0] = { 0.0f, 0.0f };
	emitter.ScaleGraph[1] = { 0.1f, 1.0f };
	emitter.ScaleGraph[2] = { 0.6f, 1.0f };
	emitter.ScaleGraph[3] = { 1.0f, 0.0f };

	emitter.BlueGraph[0] = { 0.0f, 0.0f };
	emitter.BlueGraph[1] = { 0.3f, 1.0f };
	emitter.BlueGraph[2] = { 0.7f, 0.0f };
	emitter.BlueGraph[3] = { 1.0f, 1.0f };
	
	emitter.AlphaGraph[0] = { 0.0f, 0.0f };
	emitter.AlphaGraph[1] = { 0.1f, 0.8f };
	emitter.AlphaGraph[2] = { 0.8f, 0.8f };
	emitter.AlphaGraph[3] = { 1.0f, 0.0f };

	m_Emitter = Li::MakeRef<Li::ParticleEmitter>(emitter);

	m_EmitPosition = { 0.0f, 2.0f, 0.0f };
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

	//m_EmitPosition.x += 5.0f * Li::Duration::Cast<Li::Duration::fsec>(dt).count();
	//if (m_EmitPosition.x > 10.0f)
	//	m_EmitPosition.x = -10.0f;

	if (m_BurstTimer.Update(dt))
		m_Emitter->Burst(64);

	cp::camera& camera = m_Registry.ctx<cp::camera>();


	//m_TerrainRenderer.RenderFramebuffer();
	
	Li::Renderer::BeginScene(camera.camera);

	//m_Emitter->PrintDebug("Emitter");
	m_Emitter->Update(dt, glm::translate(glm::mat4(1.0f), m_EmitPosition));
	m_Emitter->Draw(Li::ResourceManager::GetTexture2D("texture_default"));

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
