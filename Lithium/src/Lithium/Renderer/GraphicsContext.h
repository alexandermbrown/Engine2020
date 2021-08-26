#pragma once

#include "Lithium/Renderer/RendererEnums.h"
#include "glm/glm.hpp"

namespace Li
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void BindDefaultRenderTarget() = 0;
		virtual void Clear() = 0;
		virtual void CommandBarrier() = 0;
		virtual void DrawArrays(uint32_t vertex_count) = 0;
		virtual void DrawIndexed(uint32_t index_count) = 0;
		virtual void DrawIndexedInstanced(uint32_t index_count, uint32_t instance_count) = 0;
		virtual void DispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) = 0;
		virtual void ResizeView(int width, int height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetDepthTest(bool enabled) = 0;
		virtual void SetDrawMode(DrawMode mode) = 0;
		virtual void ShaderStorageBarrier() = 0;
		virtual void SwapBuffers() = 0;
		virtual void UniformBufferBarrier() = 0;
		virtual void UnbindResources(uint32_t slot, uint32_t count) = 0;
		virtual void UnbindUAVs(uint32_t slot, uint32_t count) = 0;
		virtual void UnbindVertexArray() = 0;
	};
}
