#include "pch.h"
#include "Game.h"

#include "Horizons/Core/Core.h"
#include "Horizons/Gameplay/Physics/PhysicsSystem.h"
#include "Horizons/Gameplay/Player/PlayerSystem.h"
#include "Horizons/Gameplay/Sync/SyncInitSystem.h"
#include "Horizons/Gameplay/Sync/SyncTransformSendSystem.h"
#include "Horizons/Gameplay/Sync/ConfigReceiveSystem.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Gameplay/Physics/DebugDrawSystem.h"
#endif

#include <chrono>

Game* Game::s_Instance = nullptr;

Game::Game(const TickThreadInput& data)
	: m_Running(false), m_EventQueue(data.EventQueue), m_SyncQueue(data.SyncQueue), m_TransformQueue(data.TransformQueue),
	m_ConfigStore(data.Config), m_Registry(), m_AppRun(data.Running)
{
	LI_ASSERT(!s_Instance, "Game already created!");
	s_Instance = this;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
Game::Game(const TickThreadInput& data, DebugDrawCommandQueue* debugDrawQueue)
	: m_Running(false), m_EventQueue(data.EventQueue), m_SyncQueue(data.SyncQueue), m_TransformQueue(data.TransformQueue),
	m_ConfigStore(data.Config), m_DebugDrawQueue(debugDrawQueue), m_AppRun(data.Running)
{
	LI_ASSERT(!s_Instance, "Game already created!");
	s_Instance = this;
}
#endif

Game::~Game()
{
	s_Instance = nullptr;
}

void Game::Run()
{
	m_Running = true;

	Init();

	using namespace std::chrono_literals;
	constexpr li::duration::ns dt_target(16666666ns);
	constexpr li::duration::ns sleep_margin(5ms);

	m_LastUpdateTime = std::chrono::steady_clock::now();
	
	while (m_Running && m_AppRun->load())
	{
		std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
		li::duration::ns current_delta = current_time - m_LastUpdateTime;

#ifdef HZ_SLEEP_BETWEEN_TICKS
		li::duration::ns waitNano = dt_target - current_delta - sleep_margin;
		if (waitNano > li::duration::ns::zero())
			std::this_thread::sleep_for(waitNano);
#endif
		do {
			current_time = std::chrono::steady_clock::now();
			current_delta = current_time - m_LastUpdateTime;
		} while (current_delta < dt_target);

		li::duration::ns dt;
		if (current_delta - dt_target > 16ms)
		{
			dt = current_delta;
			m_LastUpdateTime = current_time;
		}
		else
		{
			dt = dt_target;
			m_LastUpdateTime += dt_target;
		}

		if (current_delta - dt_target > 3ms)
			LI_WARN("Tick thread not at 60hz. {}ms slow", li::duration::cast<li::duration::ms>(current_delta - dt_target).count());

		SDL_Event event;
		while (m_EventQueue->try_dequeue(event))
		{
			OnEvent(&event);
		}
		
		Update(li::duration::cast<li::duration::us>(dt));
	}

	Shutdown();
}

void Game::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE)
	{
		m_Running = false;
	}
	else
	{
		if (!ConfigReceiveSystem::OnEvent(event))
		{
			PlayerSystem::OnEvent(m_Registry, event);
		}
	}
}

void Game::Init()
{
	// Initialize systems.
	SyncInitSystem::Init(m_Registry);
	PhysicsSystem::Init(m_Registry);
	PlayerSystem::Init(m_Registry, m_SyncQueue);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawSystem::Init(m_Registry, m_DebugDrawQueue);
#endif
}

void Game::Update(li::duration::us dt)
{
	PlayerSystem::Update(m_Registry, dt);
	PhysicsSystem::Step(m_Registry, dt);
	SyncTransformSendSystem::Update(m_Registry, m_TransformQueue);
#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawSystem::Draw(m_Registry);
#endif
}

void Game::Shutdown()
{
	PhysicsSystem::Shutdown(m_Registry);
#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawSystem::Shutdown(m_Registry);
#endif
}


