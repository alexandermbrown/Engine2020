#include "pch.h"
#include "SyncInitSystem.h"

#include "Article/Gameplay/Sync/Sync.h"

void SyncInitSystem::Init(entt::registry& registry)
{
	registry.set<cp::SyncManager>(0ull);
}
