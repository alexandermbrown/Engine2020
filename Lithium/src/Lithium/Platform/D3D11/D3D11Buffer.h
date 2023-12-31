#pragma once

#include "Lithium/Renderer/Buffer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Li
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(uint32_t size, BufferUsage usage);
		D3D11VertexBuffer(const float* vertices, uint32_t size, BufferUsage usage);
		virtual ~D3D11VertexBuffer() = default;

		virtual void Bind() const override {};

		virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

		virtual void SetSubData(const float* data, uint32_t size, uint32_t offset, bool discard) override;

		inline ID3D11Buffer* GetBuffer() { return m_Buffer.Get(); }

	private:
		VertexBufferLayout m_Layout;
		uint32_t m_Size;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;

		Microsoft::WRL::ComPtr<ID3D11Device> m_DeviceHandle;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(uint32_t size, BufferUsage usage);
		D3D11IndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage);
		virtual ~D3D11IndexBuffer() = default;

		virtual void Bind() const;
		virtual void Unbind() const {};

		virtual uint32_t GetCount() const { return m_Count; }

		virtual void SetSubData(const uint32_t* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		uint32_t m_Size;
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;

		Microsoft::WRL::ComPtr<ID3D11Device> m_DeviceHandle;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};
}
