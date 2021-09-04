#include "lipch.h"
#include "Pipeline.h"

#include "Lithium/Core/Application.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLPipeline.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Pipeline.h"
#endif

namespace Li
{
	Ref<Pipeline> Pipeline::Create(const Pipeline::Spec& spec)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return MakeRef<OpenGLPipeline>(spec);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return MakeRef<D3D11Pipeline>(spec);
#endif
		}

		Log::CoreError("Unknown RendererAPI!");
		return nullptr;
	}
}
