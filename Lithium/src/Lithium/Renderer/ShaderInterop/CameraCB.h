#pragma once
#ifndef LI_SI_VIEW_PROJ_CB_H
#define LI_SI_VIEW_PROJ_CB_H
#include "ShaderInterop.h"

#ifdef __cplusplus
namespace Li {
#endif

LI_CBUFFER(CameraCB, 1)
{
	aligned_float4x4 u_View;
	aligned_float4x4 u_Proj;
	aligned_float4x4 u_ViewProj;
	aligned_float3 u_CameraPos;
};

#ifdef __cplusplus
}
#endif

#endif // LI_SI_VIEW_PROJ_CB_H
