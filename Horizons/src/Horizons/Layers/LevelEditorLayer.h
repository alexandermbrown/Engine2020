#pragma once
#ifndef LI_DIST
#include "Lithium.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"

class LevelEditorLayer : public li::Layer
{
public:
	LevelEditorLayer();
	virtual ~LevelEditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

	inline bool ReturnToMainMenu() const { return m_ReturnToMainMenu; }

private:
	bool m_ReturnToMainMenu;

	entt::registry m_Registry;

	float m_BrushInnerRadius;
	float m_BrushOuterRadius;
	float m_BrushAmplitude;

	bool m_DockspaceOpen;

	// Viewport
	bool m_ViewportOpen;
	bool m_ViewportFocused;
	bool m_ViewportHovered;
	glm::ivec2 m_ViewportSize;
	li::Ref<li::Framebuffer> m_ViewportFB;

};
#endif // !LI_DIST
