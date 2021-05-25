#pragma once
#ifndef LI_SI_FRAME_CB_H
#define LI_SI_FRAME_CB_H
#include "ShaderInterop.h"

#ifdef __cplusplus
namespace Li {
#endif

	LI_CBUFFER(FrameCB, 0)
	{
		aligned_float u_RunTime;
		aligned_float u_DeltaTime;
	};

#ifdef __cplusplus
}
#endif

#endif // LI_SI_FRAME_CB_H
