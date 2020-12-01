#pragma once
#ifndef LI_DIST
#include "Horizons/Layers/LevelEditorLayer.h"

#include "Lithium.h"

class LevelEditorScene : public li::Scene
{
public:
	LevelEditorScene();
	virtual ~LevelEditorScene();

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(li::duration::us dt) override;

	virtual bool Finished() override { return true; }

private:
	LevelEditorLayer m_LevelEditorLayer;
};
#endif