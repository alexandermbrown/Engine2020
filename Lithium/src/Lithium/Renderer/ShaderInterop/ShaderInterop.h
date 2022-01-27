#pragma once
#ifndef LI_SHADERINTEROP_H
#define LI_SHADERINTEROP_H

#ifdef __cplusplus
#include "glm/glm.hpp"
#include <stdint.h>

// Inspired by WickedEngine.

#ifdef LI_PLATFORM_WINDOWS
#define LI_ALIGN_16 __declspec(align(16))
#else
#warning Test attribute alignment on other platforms.
#define LI_ALIGN_16 __attribute__((aligned(16)))
#endif

typedef uint32_t  uint;
typedef glm::ivec3 int3;
typedef glm::vec2 float2;
typedef glm::vec3 float3;
typedef glm::vec4 float4;
typedef glm::mat4 float4x4;

typedef             uint     aligned_uint;
typedef             float    aligned_float;
typedef LI_ALIGN_16 bool     aligned_bool;
typedef LI_ALIGN_16 float2   aligned_float2;
typedef LI_ALIGN_16 float3   aligned_float3;
typedef LI_ALIGN_16 float4   aligned_float4;
typedef LI_ALIGN_16 float4x4 aligned_float4x4;

#define LI_CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define LI_CBUFFER(name, slot) static const int LI_CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#else
typedef uint     aligned_uint;
typedef float    aligned_float;
typedef float2   aligned_float2;
typedef float3   aligned_float3;
typedef float4   aligned_float4;
typedef float4x4 aligned_float4x4;

#define LI_CBUFFER(name, slot) cbuffer name : register(b ## slot)
#define LI_RWStructuredBuffer(name, type, slot) RWStructuredBuffer<type> name : register(u ## slot)
#define LI_RWByteAddressBuffer(name, slot) RWByteAddressBuffer name : register(u ## slot)

#endif // __cplusplus

#endif // LI_SHADERINTEROP_H
