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
	: Layer("GameLayer"), m_BurstTimer(3000ms, false, true)
{
	m_TickThread.Begin(m_Registry);

	CameraControllerSystem::Init(m_Registry);

	m_AudioSource = Li::MakeRef<Li::AudioSource>();
	m_AudioSource->SetAudio(Li::ResourceManager::GetAudioBuffer("audio_wind"));
	m_AudioSource->Play();

	Li::EmitterProps emitter;
	emitter.MaxCount = 1024;
	emitter.Continuous = false;
	emitter.RelativeToWorld = true;
	emitter.LifeSpan = { 2.6f, 3.0f };
	emitter.SpeedRange = { 0.0f, 200.0f };
	emitter.EmitVolume = { 0.3f, 0.3f, 0.3f };
	emitter.EmitRate = 20000.0f;
	//emitter.EmitRate = emitter.MaxCount / (emitter.LifeSpan.y - (emitter.LifeSpan.y - emitter.LifeSpan.x) / 2.1f);
	emitter.ParticleScale = { 0.4f, 0.4f, 1.0f };

	emitter.AirResistance = 6.0f;

	//emitter.InitialAngle = { 0.0f, (float)M_PI / 4.0f };
	//emitter.AngularVelocity = { -1.0f, 1.0f };

	emitter.AlphaGraph[0] = { 0.0f, 0.0f };
	emitter.AlphaGraph[1] = { 0.01f, 0.7f };
	emitter.AlphaGraph[2] = { 0.8f, 0.7f };
	emitter.AlphaGraph[3] = { 1.0f, 0.0f };

	emitter.ScaleGraph[0] = { 0.0f, 0.0f };
	emitter.ScaleGraph[1] = { 0.01f, 1.0f };
	emitter.ScaleGraph[2] = { 0.8f, 1.0f };
	emitter.ScaleGraph[3] = { 1.0f, 0.0f };

	m_Emitter = Li::MakeRef<Li::ParticleEmitter>(emitter);

	m_EmitPosition = { 0.0f, 2.0f, 0.0f };
}

GameLayer::~GameLayer()
{
	m_TickThread.Finish(m_Registry);
}

void GameLayer::OnUpdate(Li::Duration::us dt)
{
	m_TickThread.UpdateSync(m_Registry, dt);

	//auto player_view = m_Registry.view<cp::sync_transform, cp::player>();
	//for (entt::entity player : player_view)
	//{
	//	cp::sync_transform& player_transform = player_view.get<cp::sync_transform>(player);
	//	break;
	//}

	CameraControllerSystem::Update(m_Registry, dt);

	TransformUpdateSystem::Update(m_Registry);

	//m_EmitPosition.x += 5.0f * Li::Duration::Cast<Li::Duration::fsec>(dt).count();
	//if (m_EmitPosition.x > 10.0f)
	//	m_EmitPosition.x = -10.0f;

	if (m_BurstTimer.Update(dt))
		m_Emitter->Burst(256);

	cp::camera& camera = m_Registry.ctx<cp::camera>();

	
	Li::Renderer::BeginScene(camera.camera.get());

	//m_Emitter->PrintDebug("Emitter");
	m_Emitter->Update(dt, glm::translate(glm::mat4(1.0f), m_EmitPosition));
	m_Emitter->Draw(Li::ResourceManager::GetTexture2D("texture_default"));

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
