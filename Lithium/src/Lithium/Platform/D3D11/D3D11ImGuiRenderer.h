#pragma once
#ifndef LI_DIST

#include "Lithium/Renderer/ImGuiRenderer.h"

namespace Li
{
	class D3D11ImGuiRenderer : public ImGuiRenderer
	{
	public:
		D3D11ImGuiRenderer();
		virtual ~D3D11ImGuiRenderer() override;

		virtual void Begin() override;
		virtual void End() override;
	};
}
#endif
