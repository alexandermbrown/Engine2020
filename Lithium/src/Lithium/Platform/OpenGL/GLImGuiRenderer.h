#pragma once
#ifndef LI_DIST
#include "Lithium/Renderer/ImGuiRenderer.h"

namespace Li
{
	class GLImGuiRenderer : public ImGuiRenderer
	{
	public:
		GLImGuiRenderer();
		virtual ~GLImGuiRenderer() override;

		virtual void Begin() override;
		virtual void End() override;
	};
}
#endif
