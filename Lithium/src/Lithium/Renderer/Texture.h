#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/RendererEnums.h"

#include <string>

namespace Li 
{
	struct TextureProps
	{
		WrapType WrapS{ WrapType::ClampToEdge };
		WrapType WrapT{ WrapType::ClampToEdge };
		FilterType MinFilter{ FilterType::Linear };
		FilterType MagFilter{ FilterType::Nearest };
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Resize(int width, int height) = 0;
		virtual void SetData(const void* data, int width, int height, bool discard) = 0;
		virtual void AttachToFramebuffer() const = 0;

		virtual void* GetInternalTexture() const = 0;
	};

	class Texture2D : public Texture {};
}
