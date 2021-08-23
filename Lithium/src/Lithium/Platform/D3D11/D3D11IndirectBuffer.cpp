#include "lipch.h"
#include "D3D11IndirectBuffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "Lithium/Core/Application.h"

namespace Li
{
	D3D11IndirectBuffer::D3D11IndirectBuffer(uint32_t size, IndirectTarget target)
		: m_Size(size)
	{
		D3D11_BUFFER_DESC desc;
		desc.StructureByteStride = 0;
		desc.ByteWidth = m_Size;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS | D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;

		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		const auto& device = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11Call(device->CreateBuffer(&desc, nullptr, &m_Buffer));

		// Create Unordered Access View
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		ZeroMemory(&uav_desc, sizeof(uav_desc));
		uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_desc.Buffer.FirstElement = 0;
		uav_desc.Buffer.NumElements = target == IndirectTarget::Compute ? 3 : 4;
		uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		D3D11Call(device->CreateUnorderedAccessView(m_Buffer.Get(), &uav_desc, m_UAV.GetAddressOf()));
	}

	void D3D11IndirectBuffer::Bind(uint32_t slot) const
	{
		m_ContextHandle->CSSetUnorderedAccessViews(slot, 1, m_UAV.GetAddressOf(), nullptr);
	}

	void D3D11IndirectBuffer::DispatchComputeIndirect(uint32_t args_offset) const
	{
		m_ContextHandle->DispatchIndirect(m_Buffer.Get(), args_offset);
	}

	void D3D11IndirectBuffer::DrawInstancedIndirect(DrawMode mode, uint32_t args_offset) const
	{
		m_ContextHandle->DrawInstancedIndirect(m_Buffer.Get(), args_offset);
	}
}
