#pragma once

#include "Lithium/Renderer/StagingBuffer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Li
{
	class D3D11StagingBuffer : public StagingBuffer
	{
	public:
		D3D11StagingBuffer(uint32_t size);
		virtual ~D3D11StagingBuffer() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Size;
	};
}
