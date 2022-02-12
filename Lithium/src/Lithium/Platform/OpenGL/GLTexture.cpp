#include "lipch.h"
#include "GLTexture.h"

#include "stb_image.h"

#include "ConvertGL.h"
#include "GLCore.h"

namespace Li
{
	GLTexture2D::GLTexture2D(int width, int height, int channels, void* data, const TextureProps& props)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = ConvertGL::TextureInternalFormat(channels);
		m_DataFormat = ConvertGL::TextureDataFormat(channels);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertGL::FilterType(props.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertGL::FilterType(props.MagFilter));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertGL::WrapType(props.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertGL::WrapType(props.WrapT));
	}

	GLTexture2D::GLTexture2D(const std::string& path, int desired_channels, const TextureProps& props)
	{
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, desired_channels);
		
		LI_CORE_ASSERT(data, "Failed to load image!");

		if (m_Channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (m_Channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else LI_CORE_ASSERT(false, "Format not supported!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertGL::FilterType(props.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertGL::FilterType(props.MagFilter));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertGL::WrapType(props.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertGL::WrapType(props.WrapT));

		stbi_image_free(data);
	}

	GLTexture2D::GLTexture2D(size_t imageSize, const uint8_t* rawData, int desired_channels, const TextureProps& props)
	{
		stbi_set_flip_vertically_on_load(1);

		 stbi_uc* data = stbi_load_from_memory(rawData, (int)imageSize, &m_Width, &m_Height, &m_Channels, 0);

		LI_CORE_ASSERT(data, "Failed to load image!");
		
		if (m_Channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (m_Channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else LI_CORE_ASSERT(false, "Format not supported!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertGL::FilterType(props.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertGL::FilterType(props.MagFilter));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertGL::WrapType(props.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertGL::WrapType(props.WrapT));

		stbi_image_free(data);
	}

	GLTexture2D::~GLTexture2D()
	{
		GLCall( glDeleteTextures(1, &m_RendererID) );
	}

	void GLTexture2D::Resize(int width, int height)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);
		m_Width = width;
		m_Height = height;
	}

	void GLTexture2D::SetData(const void* data, int width, int height, bool discard)
	{
		LI_CORE_ASSERT(width == m_Width, "Invalid width.");
		LI_CORE_ASSERT(height == m_Height, "Invalid height.");
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture2D::AttachToFramebuffer() const
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendererID, 0);
	}

	void GLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}
}
