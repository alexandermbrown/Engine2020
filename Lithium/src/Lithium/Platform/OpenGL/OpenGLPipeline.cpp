#include "lipch.h"
#include "OpenGLPipeline.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "OpenGLCore.h"

namespace Li 
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
		}

		LI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLPipeline::OpenGLPipeline(const Pipeline::Spec& spec)
		: Pipeline(spec)
	{
		glGenVertexArrays(1, &m_VAO);
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
		GLCall( glDeleteVertexArrays(1, &m_VAO) );
	}

	void OpenGLPipeline::Bind(const BindArray& vertex_buffers) const
	{
		glBindVertexArray(m_VAO);
		for (unsigned int i = 0; i < m_Spec.VertexBufferCount; i++)
		{
			const auto& vertex_buffer = vertex_buffers[i];
			LI_CORE_ASSERT(vertex_buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

			vertex_buffer->Bind();
			// bind VAO used to be here.

			const Li::VertexBufferLayout& layout = vertex_buffer->GetLayout();
			for (const Li::BufferElement& element : layout)
			{
				if (element.Type == ShaderDataType::Mat4)
				{
					for (int i = 0; i < 4; i++)
					{
						glEnableVertexAttribArray(element.Location + i);
						glVertexAttribPointer(element.Location + i,
							element.GetComponentCount() / 4,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + i * sizeof(glm::vec4)));

						if (element.Divisor > 0) {
							glVertexAttribDivisor(element.Location + i, element.Divisor);
						}
					}
				}
				else
				{
					glEnableVertexAttribArray(element.Location);
					glVertexAttribPointer(element.Location,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset));

					if (element.Divisor > 0) {
						glVertexAttribDivisor(element.Location, element.Divisor);
					}
				}
			}
		}
	}
}
