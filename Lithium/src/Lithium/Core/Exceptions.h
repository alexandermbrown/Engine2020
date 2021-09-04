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

	class AssetPackError : public std::runtime_error
	{
	public:
		AssetPackError(const char* message)
			: std::runtime_error(message) {}
		AssetPackError(const std::string& message)
			: std::runtime_error(message) {}
		virtual ~AssetPackError() = default;
	};

	class AssetError : public std::runtime_error
	{
	public:
		AssetError(const char* message)
			: std::runtime_error(message) {}
		virtual ~AssetError() = default;
	};
}
