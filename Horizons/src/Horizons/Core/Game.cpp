#include "pch.h"
#include "Game.h"

#include "Horizons/Core/Core.h"
#include "Horizons/Gameplay/Physics/PhysicsSystem.h"
#include "Horizons/Gameplay/Player/PlayerSystem.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Gameplay/Physics/DebugDrawSystem.h"
#endif

#include <chrono>

Game::Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue)
	: m_Running(false), m_EventQueue(eventQueue), m_Registry()
{
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
Game::Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, DebugDrawCommandQueue* debugDrawQueue)
	: m_Running(false), m_EventQueue(eventQueue), m_DebugDrawQueue(debugDrawQueue)
{
}
#endif

void Game::Run()
{
	m_Running = true;

	// Initialize systems.
	PhysicsSystem::Init(m_Registry);
	PlayerSystem::Init(m_Registry);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawSystem::Init(m_Registry, m_DebugDrawQueue);
#endif


	using namespace std::chrono_literals;
	constexpr float dt = 1.0f / HZ_TICKS_PER_SECOND;
	constexpr std::chrono::nanoseconds dtNano(16666666ns);
	constexpr std::chrono::nanoseconds sleepMargin(5ms);

	m_LastUpdateTime = std::chrono::steady_clock::now();
	

	while (m_Running)
	{
		std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
		std::chrono::nanoseconds diff = currentTime - m_LastUpdateTime;

#ifdef HZ_SLEEP_BETWEEN_TICKS
		std::chrono::nanoseconds waitNano = dtNano - diff - sleepMargin;
		if (waitNano.count() > 0LL) {
			
			std::this_thread::sleep_for(waitNano);
		}
#endif

		currentTime = std::chrono::steady_clock::now();
		diff = currentTime - m_LastUpdateTime;

		while (diff < dtNano)
		{
			currentTime = std::chrono::steady_clock::now();
			diff = currentTime - m_LastUpdateTime;
		}

		m_LastUpdateTime = std::chrono::steady_clock::now();
		SDL_Event event;
		while (m_EventQueue->try_dequeue(event))
		{
			OnEvent(&event);
		}
		
		PhysicsSystem::Step(m_Registry, dt);
#ifdef HZ_PHYSICS_DEBUG_DRAW
		DebugDrawSystem::Draw(m_Registry);
#endif
	}

	PhysicsSystem::Shutdown(m_Registry);
#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawSystem::Shutdown(m_Registry);
#endif

	LI_TRACE("Tick thread closing...");
}

void Game::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT)
	{
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
		{
			m_Running = false;
		}
	}
}


