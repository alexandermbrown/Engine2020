#include "lipch.h"
#include "D3D11Window.h"

#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Exceptions.h"

#include "stb_image.h"
#include "SDL2/SDL_syswm.h"

namespace Li
{
	D3D11Window::D3D11Window(const WindowProps& props)
		: m_Title(props.Title), m_Width(props.Width), m_Height(props.Height), m_Fullscreen(FullscreenType::Windowed), m_Icon(nullptr), m_IconData(nullptr)
	{
		int flags = 0;
		if (props.Resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (props.Shown)
			flags |= SDL_WINDOW_SHOWN;
		else
			flags |= SDL_WINDOW_HIDDEN;

		if (props.Borderless)
			flags |= SDL_WINDOW_BORDERLESS;

		m_Window = SDL_CreateWindow(props.Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, flags);

		if (m_Window == nullptr)
			throw SDLWindowInitError("Failed to create SDL window.");

		m_ID = SDL_GetWindowID(m_Window);

		SDL_SysWMinfo wm_info;
		SDL_VERSION(&wm_info.version);
		SDL_bool success = SDL_GetWindowWMInfo(m_Window, &wm_info);

		if (!success)
		{
			SDL_DestroyWindow(m_Window);
			throw SDLWindowInitError("Failed to get window info from SDL.");
		}

		try {
			m_Context = MakeUnique<D3D11Context>(wm_info.info.win.window, m_Width, m_Height);
		}
		catch (const GraphicsInitError& e) {
			SDL_DestroyWindow(m_Window);
			throw e;
		}
	}

	D3D11Window::~D3D11Window()
	{
		m_Context.reset();

		if (m_Icon)
			SDL_FreeSurface(m_Icon);

		if (m_IconData)
			stbi_image_free(m_IconData);

		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void D3D11Window::SwapBuffers()
	{
		m_Context->SwapBuffers();
	}

	void D3D11Window::SetVSync(bool enabled)
	{
		// TODO: setup vsync.
	}

	void D3D11Window::SetFullscreen(FullscreenType type)
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
			Log::CoreError("Unknown fullscreen type {}.", static_cast<unsigned int>(type));
			break;
		}
	}

	void D3D11Window::SetBordered(bool bordered)
	{
		SDL_SetWindowBordered(m_Window, bordered ? SDL_TRUE : SDL_FALSE);
	}

	void D3D11Window::SetResizable(bool resizable)
	{
		SDL_SetWindowResizable(m_Window, resizable ? SDL_TRUE : SDL_FALSE);
	}

	void D3D11Window::Show()
	{
		SDL_ShowWindow(m_Window);
	}

	void D3D11Window::Hide()
	{
		SDL_HideWindow(m_Window);
	}

	void D3D11Window::SetSize(int width, int height)
	{
		SDL_SetWindowSize(m_Window, width, height);
		m_Width = width;
		m_Height = height;
	}

	void D3D11Window::SetPosition(int x, int y)
	{
		SDL_SetWindowPosition(m_Window, x, y);
	}

	void D3D11Window::SetIcon(const char* path)
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

	void D3D11Window::OnWindowResize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}
}
