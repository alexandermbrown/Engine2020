#pragma once

#include <stdexcept>

namespace Li
{
	class SDLWindowInitError : public std::runtime_error
	{
	public:
		SDLWindowInitError(const char* message)
			: std::runtime_error(message) {}
		virtual ~SDLWindowInitError() = default;
	};

	class GraphicsInitError : public std::runtime_error
	{
	public:
		GraphicsInitError(const char* message)
			: std::runtime_error(message) {}
		virtual ~GraphicsInitError() = default;
	};
}
