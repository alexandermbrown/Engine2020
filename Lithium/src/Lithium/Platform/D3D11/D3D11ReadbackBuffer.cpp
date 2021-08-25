#include "lipch.h"
#include "D3D11ReadbackBuffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "D3D11ShaderBuffer.h"
#include "Lithium/Core/Application.h"

namespace Li
{
	D3D11ReadbackBuffer::D3D11ReadbackBuffer(uint32_t size)
	{
		D3D11_BUFFER_DESC desc;
		desc.StructureByteStride = 0;
		desc.ByteWidth = size;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_ContextHandle = context->GetDeviceContext();

		D3D11Call(context->GetDevice()->CreateBuffer(&desc, nullptr, &m_Buffer));
	}

	void D3D11ReadbackBuffer::Readback(Ref<ShaderBuffer> buffer, uint32_t size)
	{
		D3D11ShaderBuffer* d3d11_buffer = static_cast<D3D11ShaderBuffer*>(buffer.get());
		m_ContextHandle->CopyResource(m_Buffer.Get(), d3d11_buffer->GetBuffer().Get());
	}

	void D3D11ReadbackBuffer::GetData(void* dest, uint32_t size)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		ZeroMemory(&mapped_resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		D3D11Call(m_ContextHandle->Map(m_Buffer.Get(), 0, D3D11_MAP_READ, 0, &mapped_resource));
		memcpy(dest, mapped_resource.pData, size);
		m_ContextHandle->Unmap(m_Buffer.Get(), 0);
	}
}
