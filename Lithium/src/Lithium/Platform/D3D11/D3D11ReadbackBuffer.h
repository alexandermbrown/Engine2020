#pragma once

#include "Lithium/Renderer/ReadbackBuffer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Li
{
	class D3D11ReadbackBuffer : public ReadbackBuffer
	{
	public:
		D3D11ReadbackBuffer(uint32_t size);
		virtual ~D3D11ReadbackBuffer() = default;

		virtual void Readback(Ref<ShaderBuffer> buffer, uint32_t size) override;
		virtual void GetData(void* dest, uint32_t size) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	};
}
