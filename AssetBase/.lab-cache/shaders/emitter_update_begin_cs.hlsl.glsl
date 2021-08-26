#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(binding = 4, std140) uniform type_EmitterCB
{
    layout(row_major) mat4 u_EmitterTransform;
    vec2 u_ParticleLifeSpan;
    uint u_EmitCount;
    float u_EmitterRandomness;
} EmitterCB;

layout(binding = 4, std430) buffer type_RWByteAddressBuffer
{
    uint _m0[];
} counter_buffer;

layout(binding = 5, std430) buffer compute_iab
{
    uint _m0[];
} compute_iab_1;

uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;

void src_cs_main(uvec3 thread_id)
{
    uint dead_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_DEADCOUNT >> 2u];
    uint alive_count_new = counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION >> 2u];
    uint real_emit_count = min(dead_count, EmitterCB.u_EmitCount);
    uint _75 = COMPUTE_IAB_OFFSET_DISPATCHEMIT >> 2u;
    uvec3 _81 = uvec3(uint(ceil(float(real_emit_count) / 256.0)), 1u, 1u);
    compute_iab_1._m0[_75] = _81.x;
    compute_iab_1._m0[_75 + 1u] = _81.y;
    compute_iab_1._m0[_75 + 2u] = _81.z;
    uint _91 = COMPUTE_IAB_OFFSET_DISPATCHSIMULATION >> 2u;
    uvec3 _99 = uvec3(uint(ceil(float(alive_count_new + real_emit_count) / 256.0)), 1u, 1u);
    compute_iab_1._m0[_91] = _99.x;
    compute_iab_1._m0[_91 + 1u] = _99.y;
    compute_iab_1._m0[_91 + 2u] = _99.z;
    counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT >> 2u] = alive_count_new;
    counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION >> 2u] = 0u;
    counter_buffer._m0[PARTICLECOUNTER_OFFSET_REALEMITCOUNT >> 2u] = real_emit_count;
}

void main()
{
    THREADCOUNT_EMIT = 256u;
    THREADCOUNT_SIMULATION = 256u;
    PARTICLECOUNTER_OFFSET_ALIVECOUNT = 0u;
    PARTICLECOUNTER_OFFSET_DEADCOUNT = PARTICLECOUNTER_OFFSET_ALIVECOUNT + 4u;
    PARTICLECOUNTER_OFFSET_REALEMITCOUNT = PARTICLECOUNTER_OFFSET_DEADCOUNT + 4u;
    PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION = PARTICLECOUNTER_OFFSET_REALEMITCOUNT + 4u;
    COMPUTE_IAB_OFFSET_DISPATCHEMIT = 0u;
    COMPUTE_IAB_OFFSET_DISPATCHSIMULATION = COMPUTE_IAB_OFFSET_DISPATCHEMIT + 12u;
    DRAW_IAB_OFFSET_DRAWPARTICLES = 0u;
    uvec3 param_var_thread_id;
    src_cs_main(param_var_thread_id);
}

