#pragma once

#include "Lithium/Core/Layer.h"
#include "Lithium/Renderer/Texture.h"

#include "glm/glm.hpp"

class SplashScreenLayer : public Li::Layer
{
public:
	SplashScreenLayer();
	virtual ~SplashScreenLayer() = default;

	virtual void OnUpdate(Li::Duration::us dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

private:
	void CalculateTransform();

	Li::Ref<Li::Texture2D> m_Texture;
	glm::mat4 m_Transform;
};
