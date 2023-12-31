#pragma once

#include "Article/Config/ConfigStore.h"
#include "Article/Gameplay/Sync/Sync.h"
#include "Article/Gameplay/Sync/SyncTransform.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#	include "Article/Rendering/DebugPhysicsDrawShared.h"
#endif

#include "Lithium/Utility/Time.h"
#include "SDL2/SDL.h"
#include "entt/entt.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

#include <thread>
#include <atomic>

struct TickThreadInput
{
	std::atomic<bool>* Running;
	moodycamel::ReaderWriterQueue<SDL_Event>* EventQueue;
	SyncEventQueue* SyncQueue;
	SyncTransformQueue* TransformQueue;
	ConfigStore Config;
};

class TickThread
{
public:
	TickThread();
	void Begin(entt::registry& registry);
	void Finish(entt::registry& registry);

	void UpdateSync(entt::registry& registry, Li::Duration::us dt);
	void OnEvent(SDL_Event* event);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue* GetDebugDrawQueue() { return &m_DebugDrawQueue; }
#endif

private:
	static int ThreadEntryPoint(const TickThreadInput& data);
#ifdef HZ_PHYSICS_DEBUG_DRAW
	static int ThreadEntryPointDebugDraw(const TickThreadInput& data, DebugDrawCommandQueue* debug_draw_queue);
#endif

	std::thread m_TickThread;
	moodycamel::ReaderWriterQueue<SDL_Event> m_EventQueue;
	SyncEventQueue m_SyncQueue;
	SyncTransformQueue m_TransformQueue;
	std::atomic<bool> m_ThreadRun;
	bool m_Started;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue m_DebugDrawQueue;
#endif
};
