#include "lipch.h"
#include "ReadbackBuffer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Core/Assert.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLReadbackBuffer.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11ReadbackBuffer.h"
#endif

namespace Li
{
    Ref<ReadbackBuffer> ReadbackBuffer::Create(uint32_t size)
    {
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:  return MakeRef<OpenGLReadbackBuffer>(size);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:  return MakeRef<D3D11ReadbackBuffer>(size);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}
