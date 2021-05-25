#pragma once
#ifndef HZ_SI_ATLAS_BOUNDS_CB_H
#define HZ_SI_ATLAS_BOUNDS_CB_H
#include "Lithium/Renderer/ShaderInterop/ShaderInterop.h"

LI_CBUFFER(AtlasBoundsCB, 5)
{
	aligned_float4 u_AtlasBounds0;
	aligned_float4 u_AtlasBounds1;
	aligned_float4 u_AtlasBounds2;
	aligned_float4 u_AtlasBounds3;
	aligned_float3 u_NoiseWeights;
	aligned_float3 u_BlendWidths;
};

#endif // HZ_SI_ATLAS_BOUNDS_CB_H
