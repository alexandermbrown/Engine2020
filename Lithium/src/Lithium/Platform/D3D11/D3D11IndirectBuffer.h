#pragma once

#include "Lithium/Renderer/IndirectBuffer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Li
{
	class D3D11IndirectBuffer : public IndirectBuffer
	{
	public:
		D3D11IndirectBuffer(uint32_t size, uint32_t stride);

		virtual void Bind(uint32_t slot) const override;
		virtual void DispatchComputeIndirect(uint32_t args_offset) const override;
		virtual void DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;

		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;

		uint32_t m_Size;
	};
}
