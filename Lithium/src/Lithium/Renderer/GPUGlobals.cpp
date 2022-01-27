#include "lipch.h"

// Prefer dedicated GPUs.
extern "C" {
#ifdef LI_PLATFORM_WINDOWS
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
}
