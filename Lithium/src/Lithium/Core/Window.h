#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/GraphicsContext.h"

#include "SDL2/SDL.h"

namespace Li
{
	struct WindowProps
	{
		const char* Title;
		int Width;
		int Height;
		bool Resizable;
		bool Shown;
		bool Borderless;
	};

	enum class FullscreenType
	{
		Windowed,
		Fullscreen,
		FullscreenWindowed
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void SwapBuffers() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual GraphicsContext* GetContext() = 0;
		virtual SDL_Window* GetWindow() const = 0;
		virtual uint32_t GetWindowID() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetFullscreen(FullscreenType type) = 0;
		virtual FullscreenType GetFullscreen() const = 0;

		virtual void SetBordered(bool bordered) = 0;
		virtual void SetResizable(bool resizable) = 0;

		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual void SetSize(int width, int height) = 0;
		virtual void SetPosition(int x, int y) = 0;

		virtual void SetIcon(const char* path) = 0;

		virtual void OnWindowResize(int width, int height) = 0;
	};
}
