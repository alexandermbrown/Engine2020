#include "lipch.h"
#include "IndirectBuffer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Core/Assert.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLIndirectBuffer.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11IndirectBuffer.h"
#endif

namespace Li
{
	Ref<IndirectBuffer> IndirectBuffer::Create(uint32_t size, IndirectTarget target)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:  return MakeRef<OpenGLIndirectBuffer>(size, target);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:  return MakeRef<D3D11IndirectBuffer>(size, target);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
