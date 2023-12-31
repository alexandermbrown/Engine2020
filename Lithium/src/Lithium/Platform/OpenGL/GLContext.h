#pragma once

#include "Lithium/Renderer/GraphicsContext.h"
#include "SDL2/SDL.h"

namespace Li
{
	class GLContext : public GraphicsContext
	{
	public:
		GLContext(SDL_Window* windowHandle, int width, int height);
		virtual ~GLContext();

		virtual void BindDefaultRenderTarget() override;
		virtual void Clear() override;
		virtual void CommandBarrier() override;
		virtual void DrawArrays(uint32_t vertexCount) override;
		virtual void DrawIndexed(uint32_t indexCount) override;
		virtual void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount) override;
		virtual void DispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) override;
		virtual void ResizeView(int width, int height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetDepthTest(bool enabled) override;
		virtual void SetDrawMode(DrawMode mode) override { m_DrawMode = mode; }
		virtual void ShaderStorageBarrier() override;
		virtual void SwapBuffers() override;
		virtual void UniformBufferBarrier() override;
		virtual void UnbindResources(uint32_t slot, uint32_t count) override {};
		virtual void UnbindUAVs(uint32_t slot, uint32_t count) override {};
		virtual void UnbindVertexArray() override;

		inline SDL_GLContext* GetGLContext() { return &m_Context; }

	private:
		SDL_Window* m_WindowHandle;
		SDL_GLContext m_Context;
		DrawMode m_DrawMode;

		int m_Width;
		int m_Height;

		bool m_DepthTest;
	};
}
