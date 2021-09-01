#include "lipch.h"
#include "AudioContext.h"

#include "Lithium/Core/Memory.h"

#define AL_LIBTYPE_STATIC
#include "AL/alc.h"

namespace Li
{
	AudioContext::AudioContext(const char* device_name)
		: m_Device(nullptr), m_Context(nullptr)
	{
		// TODO: Enumerate through devices and allow user to change the device.
		ALCdevice* device = alcOpenDevice(device_name);
		if (device == nullptr)
		{
			throw AudioInitError("Failed to open audio device.");
		}
		ALCcontext* context = alcCreateContext(device, nullptr);
		if (context == nullptr)
		{
			alcCloseDevice(device);
			throw AudioInitError("Failed to create audio context.");
		}
		if (alcMakeContextCurrent(context) == false)
		{
			alcDestroyContext(context);
			alcCloseDevice(device);
			throw AudioInitError("Failed to make audio context current!");
		}

		m_Device = device;
		m_Context = context;
	}

	AudioContext::~AudioContext()
	{
		ALCdevice* device = static_cast<ALCdevice*>(m_Device);
		ALCcontext* context = static_cast<ALCcontext*>(m_Context);

		alcMakeContextCurrent(nullptr);
		if (context)
			alcDestroyContext(context);
		if (device)
			alcCloseDevice(device);
	}
}
