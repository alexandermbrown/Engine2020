#pragma once

#include "Lithium/Core/Core.h"

#ifdef LI_ENABLE_ASSERTS
#	define ALCall(x) ALClearError();\
	x;\
	LI_CORE_ASSERT(ALCheckError(#x, __FILE__, __LINE__), "OpenAL error!")

namespace li
{
	void ALClearError();
	bool ALCheckError(const char* function, const char* file, int line);
}
#else
#	define ALCall(x) x
#endif