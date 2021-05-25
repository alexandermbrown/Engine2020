#pragma once

#include "Article/Core/TickThread.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Article/Rendering/DebugPhysicsDrawShared.h"
#include "Article/Rendering/DebugPhysicsRenderer.h"
#endif

#include "Article/Terrain/TerrainRenderer.h"
#include "Article/Terrain/DefaultTerrainStore.h"

#include "Lithium.h"
#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

class GameLayer : public Li::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnUpdate(Li::Duration::us dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

private:
	Li::Ref<Li::AudioSource> m_AudioSource;

	entt::registry m_Registry;

	TickThread m_TickThread;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugPhysicsRenderer m_DebugPhysicsRenderer;
#endif

	Li::Ref<Li::ParticleEmitter> m_Emitter;

	//DefaultTerrainStore m_TerrainStore;
	//TerrainRenderer m_TerrainRenderer;
};
