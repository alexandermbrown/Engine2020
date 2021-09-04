#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Pipeline.h"
#include "Lithium/Renderer/Shader.h"
#include "D3D11Buffer.h"

#include <d3d11.h>
#include <wrl/client.h>

#include <vector>

namespace Li
{
	class D3D11Pipeline : public Pipeline
	{
	public:
		D3D11Pipeline(const Pipeline::Spec& spec);
		virtual ~D3D11Pipeline() = default;

		virtual void Bind(const BindArray& vertex_buffers) const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};
}
