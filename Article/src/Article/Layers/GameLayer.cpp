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

#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
using namespace std::chrono_literals;

GameLayer::GameLayer()
	: Layer("GameLayer")
{
	m_TickThread.Begin(m_Registry);

	CameraControllerSystem::Init(m_Registry);

	Li::Unique<Li::Model> teapot_model = Li::MakeUnique<Li::Model>("./data/models/model_teapot.lmodel");
	Li::Unique<Li::Model> scene_model = Li::MakeUnique<Li::Model>("./data/models/model_scene.lmodel");

	entt::entity scene = m_Registry.create();
	m_Registry.emplace<cp::Model>(scene, std::move(scene_model));
	cp::Transform& scene_transform = m_Registry.emplace<cp::Transform>(scene);
	scene_transform.rotation = glm::quat({ 0.0f, 0.0f, (float)M_PI / 4.0f });
	scene_transform.old = true;

	entt::entity teapot = m_Registry.create();
	m_Registry.emplace<cp::Model>(teapot, std::move(teapot_model));
	m_Registry.emplace<cp::Transform>(teapot);
}

GameLayer::~GameLayer()
{
	m_TickThread.Finish(m_Registry);
}

void GameLayer::OnUpdate(Li::Duration::us dt)
{
	m_TickThread.UpdateSync(m_Registry, dt);

	CameraControllerSystem::Update(m_Registry, dt);

	TransformUpdateSystem::Update(m_Registry);

	cp::Camera& camera = m_Registry.ctx<cp::Camera>();

	Li::Renderer::BeginScene(camera.camera_ptr.get());

	RenderingSystem::Render(m_Registry);

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
