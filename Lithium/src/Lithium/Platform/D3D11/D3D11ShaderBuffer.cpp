#include "lipch.h"
#include "D3D11ShaderBuffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "Lithium/Core/Application.h"

namespace Li
{
	D3D11ShaderBuffer::D3D11ShaderBuffer(const void* data, uint32_t size, uint32_t stride, ShaderBufferType type)
		: m_Size(size)
	{
		D3D11_BUFFER_DESC desc;
		desc.StructureByteStride = stride;
		desc.ByteWidth = m_Size;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		switch (type)
		{
		case ShaderBufferType::Structured:
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			LI_CORE_ASSERT(stride == sizeof(uint32_t) || stride % 16 == 0);
			break;
		case ShaderBufferType::Raw:
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
			break;
		default:
			LI_CORE_ASSERT(false, "Unknown shader buffer type.");
		}
		
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		const auto& device = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();
		 
		if (data)
		{
			D3D11_SUBRESOURCE_DATA buffer_data;
			buffer_data.pSysMem = data;
			buffer_data.SysMemPitch = 0;
			buffer_data.SysMemSlicePitch = 0;
			D3D11Call(device->CreateBuffer(&desc, &buffer_data, m_Buffer.GetAddressOf()));
		}
		else
		{
			D3D11Call(device->CreateBuffer(&desc, nullptr, m_Buffer.GetAddressOf()));
		}
		
		// Create Shader Resource View
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		ZeroMemory(&srv_desc, sizeof(srv_desc));
		if (type == ShaderBufferType::Structured)
		{
			srv_desc.Format = DXGI_FORMAT_UNKNOWN;
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srv_desc.Buffer.FirstElement = 0;
			srv_desc.Buffer.NumElements = size / stride;
		}
		else
		{
			srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			srv_desc.BufferEx.FirstElement = 0;
			srv_desc.BufferEx.NumElements = size / stride;
			srv_desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		}
		D3D11Call(device->CreateShaderResourceView(m_Buffer.Get(), &srv_desc, m_SRV.GetAddressOf()));

		// Create Unordered Access View
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		ZeroMemory(&uav_desc, sizeof(uav_desc));
		
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_desc.Buffer.FirstElement = 0;
		uav_desc.Buffer.NumElements = size / stride;
		
		if (type == ShaderBufferType::Raw)
		{
			LI_CORE_ASSERT(stride == sizeof(uint32_t));
			uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
			uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		}
		else
		{
			uav_desc.Format = DXGI_FORMAT_UNKNOWN;
		}
		
		D3D11Call(device->CreateUnorderedAccessView(m_Buffer.Get(), &uav_desc, m_UAV.GetAddressOf()));
	}

	void D3D11ShaderBuffer::BindSRV(ShaderType type, uint32_t slot) const
	{
		switch (type)
		{
		case ShaderType::Vertex:
			m_ContextHandle->VSSetShaderResources(slot, 1, m_SRV.GetAddressOf());
			break;
		case ShaderType::Geometry:
			m_ContextHandle->GSSetShaderResources(slot, 1, m_SRV.GetAddressOf());
			break;
		case ShaderType::Fragment:
			m_ContextHandle->PSSetShaderResources(slot, 1, m_SRV.GetAddressOf());
			break;
		case ShaderType::Compute:
			m_ContextHandle->CSSetShaderResources(slot, 1, m_SRV.GetAddressOf());
			break;
		default:
			LI_CORE_ASSERT(false, "Unsupported shader type.");
		}
	}

	void D3D11ShaderBuffer::BindUAV(uint32_t slot) const
	{
		m_ContextHandle->CSSetUnorderedAccessViews(slot, 1, m_UAV.GetAddressOf(), nullptr);
	}
}
