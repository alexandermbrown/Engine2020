#pragma once

#include "Lithium/Renderer/ShaderBuffer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Li
{
	class D3D11ShaderBuffer : public ShaderBuffer
	{
	public:
		D3D11ShaderBuffer(const void* data, uint32_t size, uint32_t stride, ShaderBufferType type);

		virtual void BindBase(uint32_t slot) const override {};
		virtual void BindSRV(ShaderType type, uint32_t slot) const override;
		virtual void BindUAV(uint32_t slot) const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;

		uint32_t m_Size;
	};
}
