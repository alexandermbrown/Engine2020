#pragma once

#include "Lithium/Renderer/Framebuffer.h"
#include "Lithium/Platform/OpenGL/GLRenderbuffer.h"

namespace Li
{
	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(int width, int height);
		virtual ~GLFramebuffer();

		virtual void Bind() const override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() const override;

		inline virtual Ref<Texture2D> GetTexture() const override { return m_Texture; };
		virtual void Resize(int width, int height) override;
		virtual glm::ivec2 GetSize() const override { return { m_Width, m_Height }; };

	private:

		uint32_t m_RendererID;
		Ref<Texture2D> m_Texture;
		Unique<GLRenderbuffer> m_Renderbuffer;

		int m_Width;
		int m_Height;
	};
}