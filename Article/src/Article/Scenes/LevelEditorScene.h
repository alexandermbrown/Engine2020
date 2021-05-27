#pragma once
#ifndef LI_DIST
#include "Article/Layers/LevelEditorLayer.h"

#include "Lithium/Core/Scene.h"

class LevelEditorScene : public Li::Scene
{
public:
	LevelEditorScene();
	virtual ~LevelEditorScene();

	virtual void OnShow() override;
	virtual void OnTransition() override;
};
#endif
