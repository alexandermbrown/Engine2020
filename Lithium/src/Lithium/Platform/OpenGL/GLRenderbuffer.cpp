#include "lipch.h"
#include "GLRenderbuffer.h"

#include "ConvertGL.h"
#include "glad/glad.h"

#include "GLCore.h"

namespace Li
{
	GLRenderbuffer::GLRenderbuffer(uint32_t width, uint32_t height)
	{
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	GLRenderbuffer::~GLRenderbuffer()
	{
		GLCall( glDeleteRenderbuffers(1, &m_RendererID) );
	}

	void GLRenderbuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	}

	void GLRenderbuffer::Unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void GLRenderbuffer::Resize(uint32_t width, uint32_t height)
	{
		glDeleteRenderbuffers(1, &m_RendererID);
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	void GLRenderbuffer::AttachToFramebuffer() const
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererID);
	}
}