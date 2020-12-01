#pragma once

#include "Lithium.h"

#include "Horizons/Layers/MainMenuLayer.h"
#include "Horizons/Layers/DiagnosticsLayer.h"

class MainMenuScene : public li::Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(li::duration::us dt) override;

	virtual bool Finished() override { return m_MainMenuLayer.IsFinished(); }

private:
	MainMenuLayer m_MainMenuLayer;
#ifndef LI_DIST
	DiagnosticsLayer m_Diagnostics;
#endif
};
