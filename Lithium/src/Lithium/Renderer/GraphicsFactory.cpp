#include "lipch.h"
#include "GraphicsFactory.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/GLGraphicsFactory.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11GraphicsFactory.h"
#endif

namespace Li
{
	GraphicsFactory* GraphicsFactory::s_Instance = nullptr;

	GraphicsFactory::GraphicsFactory()
	{
		LI_CORE_ASSERT(s_Instance == nullptr, "GraphicsFactory instance being overwritten.");
		s_Instance = this;
	}

	Unique<GraphicsFactory> GraphicsFactory::Create(GraphicsAPI api)
	{
		switch (api)
		{
#ifdef LI_INCLUDE_OPENGL
		case GraphicsAPI::OpenGL:
			return MakeUnique<GLGraphicsFactory>();
#endif
#ifdef LI_INCLUDE_D3D11
		case GraphicsAPI::D3D11:
			return MakeUnique<D3D11GraphicsFactory>();
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
