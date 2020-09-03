#include "lipch.h"
#include "Window.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLWindow.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Window.h"
#endif

namespace li 
{
	Window* Window::Create(const WindowProps& props)
	{
		switch (props.API)
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return new OpenGLWindow(props);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return new D3D11Window(props);
#endif
		default:
			LI_CORE_ERROR("Unknown graphics API");
			return nullptr;
		}
	}
}

