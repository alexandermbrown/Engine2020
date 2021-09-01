#pragma once

namespace Li
{
	class AudioContext
	{
	public:
		// Set device_name to nullptr to automatically pick a device.
		AudioContext(const char* device_name);
		~AudioContext();

	private:
		/*ALCdevice*/ void* m_Device;
		/*ALCcontext*/ void* m_Context;
	};

	class AudioInitError : public std::runtime_error
	{
	public:
		AudioInitError(const char* message)
			: std::runtime_error(message) {}

		virtual ~AudioInitError() = default;
	};
}
