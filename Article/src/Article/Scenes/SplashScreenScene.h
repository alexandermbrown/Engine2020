#pragma once

#include "Lithium/Core/Scene.h"

class SplashScreenScene : public Li::Scene
{
public:
	virtual ~SplashScreenScene() = default;

	virtual void OnShow() override;
	virtual void OnTransition() override;
};
