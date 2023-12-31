#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Core/Log.h"
#include "Lithium/Renderer/RendererEnums.h"

#include <vector>

namespace Li 
{
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		}

		Log::CoreError("Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Location;
		size_t Offset;
		bool Normalized;
		unsigned int Divisor;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, uint32_t location, bool normalized = false, unsigned int divisor = 0)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Location(location), Offset(0), Normalized(normalized), Divisor(divisor)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			}

			Log::CoreError("Unknown ShaderDataType!");
			return 0;
		}

		uint32_t GetSlotCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 1;
			case ShaderDataType::Float3:  return 1;
			case ShaderDataType::Float4:  return 1;
			case ShaderDataType::Mat4:    return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 1;
			case ShaderDataType::Int3:    return 1;
			case ShaderDataType::Int4:    return 1;
			}

			Log::CoreError("Unknown ShaderDataType!");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {}

		VertexBufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		uint32_t GetComponentCount() const;

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual void SetSubData(const float* data, uint32_t size, uint32_t offset, bool discard) = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual void SetSubData(const uint32_t* data, uint32_t size, uint32_t offset, bool discard) = 0;
	};
}
