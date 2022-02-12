#include "lipch.h"
#include "GLWindow.h"

#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Application.h"
#include "Lithium/Core/Exceptions.h"

#include "stb_image.h"

namespace Li
{
	GLWindow::GLWindow(const WindowProps& props)
		: m_Title(props.Title), m_Width(props.Width), m_Height(props.Height), m_VSync(false), m_Fullscreen(FullscreenType::Windowed), m_Icon(nullptr), m_IconData(nullptr)
	{
		int flags = SDL_WINDOW_OPENGL;
		if (props.Resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (props.Shown)
			flags |= SDL_WINDOW_SHOWN;
		else
			flags |= SDL_WINDOW_HIDDEN;

		if (props.Borderless)
			flags |= SDL_WINDOW_BORDERLESS;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		m_Window = SDL_CreateWindow(props.Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, flags);
		
		if (m_Window == nullptr) {
			Log::CoreFatal("SDL ERROR: {}", SDL_GetError());
			throw SDLWindowInitError("Failed to create SDL window.");
		}

		m_ID = SDL_GetWindowID(m_Window);

		m_Context = MakeUnique<GLContext>(m_Window, m_Width, m_Height);
	}

	GLWindow::~GLWindow()
	{
		m_Context.reset();

		if (m_Icon)
			SDL_FreeSurface(m_Icon);

		if (m_IconData)
			stbi_image_free(m_IconData);

		SDL_DestroyWindow(m_Window);
	}

	void GLWindow::SwapBuffers()
	{
		m_Context->SwapBuffers();
	}

	void GLWindow::SetVSync(bool enabled)
	{
		if (SDL_GL_SetSwapInterval(enabled ? -1 : 0) < 0)
			Log::CoreError("Error setting VSync to {0}", enabled);
		m_VSync = enabled;
	}

	void GLWindow::SetFullscreen(FullscreenType type)
	{
		m_Fullscreen = type;
		switch (m_Fullscreen)
		{
		case Li::FullscreenType::Windowed:
		{
			LI_CORE_VERIFY(!SDL_SetWindowFullscreen(m_Window, 0), "Failed to turn to windowed.");
			break;
		}
		case Li::FullscreenType::Fullscreen:
		{
			LI_CORE_VERIFY(!SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN), "Failed to turn to fullscreen.");
			break;
		}
		case Li::FullscreenType::FullscreenWindowed:
		{
			LI_CORE_VERIFY(!SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP), "Failed to turn to fullscreen windowed.");
			break;
		}
		default:
			LI_CORE_ASSERT(false, "Unknown fullscreen type.");
			break;
		}
	}

	void GLWindow::SetBordered(bool bordered)
	{
		SDL_SetWindowBordered(m_Window, bordered ? SDL_TRUE : SDL_FALSE);
	}

	void GLWindow::SetResizable(bool resizable)
	{
		SDL_SetWindowResizable(m_Window, resizable ? SDL_TRUE : SDL_FALSE);
	}

	void GLWindow::Show()
	{
		SDL_ShowWindow(m_Window);
	}

	void GLWindow::Hide()
	{
		SDL_HideWindow(m_Window);
	}

	void GLWindow::SetSize(int width, int height)
	{
		SDL_SetWindowSize(m_Window, width, height);
		m_Width = width;
		m_Height = height;
	}

	void GLWindow::SetPosition(int x, int y)
	{
		SDL_SetWindowPosition(m_Window, x, y);
	}

	void GLWindow::SetIcon(const char* path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);

		m_IconData = stbi_load(path, &width, &height, &channels, 0);

		LI_CORE_ASSERT(m_IconData, "Failed to load image!");

		int pitch = 0;
		uint32_t pixel_format = 0;
		if (channels == 4)
		{
			pitch = 4 * width;
			pixel_format = SDL_PIXELFORMAT_RGBA32;
		}
		else if (channels == 3)
		{
			pitch = 3 * width;
			pixel_format = SDL_PIXELFORMAT_RGB24;
		}
		else
		{
			Log::CoreError("Image must have 3 or 4 chanels.");
			return;
		}

		m_Icon = SDL_CreateRGBSurfaceWithFormatFrom(m_IconData, width, height, channels * 8, pitch, pixel_format);

		if (!m_Icon)
		{
			Log::CoreError("Failed to create SDL_Surface.");
			return;
		}

		SDL_SetWindowIcon(m_Window, m_Icon);

		Log::CoreInfo("Set icon to {}", path);
	}

	void GLWindow::OnWindowResize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}
}
