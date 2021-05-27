#pragma once

#include "Lithium/Core/Scene.h"

class GameScene : public Li::Scene
{
public:
	virtual ~GameScene() = default;

	virtual void OnShow() override;
	virtual void OnTransition() override;
};
