#pragma once

#include "Article/Core/TickThread.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Article/Rendering/DebugPhysicsDrawShared.h"
#include "Article/Rendering/DebugPhysicsRenderer.h"
#endif

#include "Lithium/Core/Layer.h"
#include "Lithium/Renderer/Model.h"
#include "Lithium/Renderer/ParticleEmitter.h"
#include "Lithium/Utility/Timer.h"
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
	entt::registry m_Registry;

	TickThread m_TickThread;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugPhysicsRenderer m_DebugPhysicsRenderer;
#endif
};
