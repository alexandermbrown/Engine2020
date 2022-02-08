#include "lipch.h"
#include "D3D11Pipeline.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Core/Assert.h"
#include "D3D11Core.h"
#include "D3D11Context.h"
#include "D3D11Shader.h"

namespace Li
{
	static DXGI_FORMAT ShaderDataTypeToD3D11(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Mat4:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
			LI_CORE_ASSERT(false, "Bool not supported!");
			return DXGI_FORMAT_UNKNOWN;
		}
		LI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}

	D3D11Pipeline::D3D11Pipeline(const Pipeline::Spec& spec)
		: Pipeline(spec)
	{
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_ContextHandle = context->GetDeviceContext();

		std::vector<D3D11_INPUT_ELEMENT_DESC> input_desc = std::vector<D3D11_INPUT_ELEMENT_DESC>();

		int slot = 0;
		for (unsigned int i = 0; i < m_Spec.VertexBufferCount; i++)
		{
			for (const BufferElement& element : m_Spec.Layouts[i].GetElements())
			{
				if (element.Type == ShaderDataType::Mat4)
				{
					for (int row = 0; row < 4; row++)
					{
						D3D11_INPUT_ELEMENT_DESC desc;
						desc.SemanticName = element.Name.c_str();
						desc.SemanticIndex = row;
						desc.Format = ShaderDataTypeToD3D11(element.Type);
						desc.InputSlot = slot;
						desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
						desc.InputSlotClass = element.Divisor > 0 ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
						desc.InstanceDataStepRate = element.Divisor;
						input_desc.push_back(desc);
					}
				}
				else
				{
					D3D11_INPUT_ELEMENT_DESC desc;
					desc.SemanticName = element.Name.c_str();
					desc.SemanticIndex = 0;
					desc.Format = ShaderDataTypeToD3D11(element.Type);
					desc.InputSlot = slot;
					desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
					desc.InputSlotClass = element.Divisor > 0 ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
					desc.InstanceDataStepRate = element.Divisor;
					input_desc.push_back(desc);
				}
			}
			slot++;
		}

		LI_CORE_ASSERT(input_desc.size() < 32, "Layout too large!");

		D3D11Shader* d3d_shader = (D3D11Shader*)m_Spec.ShaderRef.get();
		D3D11Call(context->GetDevice()->CreateInputLayout(
			input_desc.data(),
			(uint32_t)input_desc.size(),
			d3d_shader->GetVSBufferData(),
			d3d_shader->GetVSBufferSize(),
			&m_InputLayout));
	}

	void D3D11Pipeline::Bind(const BindArray& vertex_buffers) const
	{
		std::array<ID3D11Buffer*, MaxVertexBufferCount> d3d_vertex_buffers;
		std::array<UINT, MaxVertexBufferCount> strides;
		std::array<UINT, MaxVertexBufferCount> offsets;
		for (unsigned int i = 0; i < m_Spec.VertexBufferCount; i++)
		{
			D3D11VertexBuffer* d3d_vb = (D3D11VertexBuffer*)vertex_buffers[i].get();
			d3d_vertex_buffers[i] = d3d_vb->GetBuffer();
			strides[i] = d3d_vb->GetLayout().GetStride();
			offsets[i] = 0;
		}

		m_ContextHandle->IASetInputLayout(m_InputLayout.Get());
		m_ContextHandle->IASetVertexBuffers(0, m_Spec.VertexBufferCount,
			d3d_vertex_buffers.data(), strides.data(), offsets.data());
	}
}
