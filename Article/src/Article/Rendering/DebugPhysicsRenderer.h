#pragma once

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "DebugPhysicsDrawShared.h"

#include <queue>

class DebugPhysicsRenderer
{
public:
	void SubmitLines(DebugDrawCommandQueue* queue);

private:
	constexpr static float ZPos = 0.0f;

	std::deque<DebugDrawCommand> m_FeedQueue;
	std::deque<DebugDrawCommand> m_CurrentQueue;
};
#endif
