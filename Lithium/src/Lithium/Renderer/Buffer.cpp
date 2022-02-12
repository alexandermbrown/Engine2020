#include "lipch.h"
#include "Buffer.h"

namespace Li
{

	uint32_t VertexBufferLayout::GetComponentCount() const
	{
		uint32_t count = 0;
		for (const BufferElement& element : m_Elements)
		{
			count += element.GetComponentCount();
		}
		return count;
	}
}