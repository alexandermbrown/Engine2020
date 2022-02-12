#include "lipch.h"
#include "GLFramebuffer.h"

#include "Lithium/Renderer/GraphicsFactory.h"

#include "GLCore.h"
#include "ConvertGL.h"
#include "glad/glad.h"

namespace Li
{
	GLFramebuffer::GLFramebuffer(int width, int height)
		: m_Width(width), m_Height(height)
	{
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		TextureProps props {
			WrapType::ClampToEdge, WrapType::ClampToEdge,
			FilterType::Linear, FilterType::Nearest
		};
		m_Texture = GraphicsFactory::Get()->MakeTexture2D(width, height, 4, nullptr, props);
		m_Texture->AttachToFramebuffer();

		m_Renderbuffer = MakeUnique<GLRenderbuffer>(width, height);
		m_Renderbuffer->AttachToFramebuffer();

		LI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer not complete!");
	}

	GLFramebuffer::~GLFramebuffer()
	{
		GLCall( glDeleteFramebuffers(1, &m_RendererID) );
	}

	void GLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
	}

	void GLFramebuffer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLFramebuffer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLFramebuffer::Resize(int width, int height)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		m_Width = width;
		m_Height = height;

		m_Texture->Bind();
		m_Texture->Resize(width, height);
		m_Texture->AttachToFramebuffer();

		m_Renderbuffer->Resize(width, height);
		m_Renderbuffer->AttachToFramebuffer();
	}
}