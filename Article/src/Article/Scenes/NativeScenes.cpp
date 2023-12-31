#include "pch.h"
#include "NativeScenes.h"

#include "GameScene.h"
#include "VideoPlayerScene.h"

Li::Unique<Li::Scene> GetNativeScene(const std::string& name)
{
	if (name == "GameScene")
		return Li::MakeUnique<GameScene>();
#ifndef LI_DIST
	else if (name == "VideoPlayerScene")
		return Li::MakeUnique<VideoPlayerScene>();
#endif

	Li::Log::Error("Unknown native scene {}", name);
	return nullptr;
}
