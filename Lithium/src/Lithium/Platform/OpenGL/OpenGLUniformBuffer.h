#pragma once

#include "Lithium/Core/Assert.h"
#include "Lithium/Renderer/UniformBuffer.h"
#include "OpenGLBuffer.h"

#include <unordered_map>
#include <string>

namespace Li
{
	class OpenGLUniformBuffer : public UniformBuffer, public OpenGLBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t slot, uint32_t size);
		virtual ~OpenGLUniformBuffer() = default;

		virtual void Bind(ShaderType type) const override {};
		virtual void BindBase() const override;
		virtual inline uint32_t GetSlot() const override { return m_Slot; }
		virtual void SetData(const void* data) override;

	private:
		uint32_t m_Slot;
		uint32_t m_Size;
	};
}
