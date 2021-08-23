#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(binding = 4, std140) uniform type_EmitterCB
{
    layout(row_major) mat4 u_EmitterTransform;
    uint u_EmitCount;
    float u_EmitterRandomness;
    vec2 u_ParticleLifeSpan;
} EmitterCB;

layout(binding = 4, std430) readonly buffer type_ByteAddressBuffer
{
    uint _m0[];
} counter_buffer;

layout(binding = 5, std430) buffer type_RWByteAddressBuffer
{
    uint _m0[];
} indirect_buffers;

uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;

void src_cs_main(uvec3 DTid)
{
    uint particle_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION >> 2u];
    uint _64 = DRAW_IAB_OFFSET_DRAWPARTICLES >> 2u;
    uvec4 _67 = uvec4(4u, particle_count, 0u, 0u);
    indirect_buffers._m0[_64] = _67.x;
    indirect_buffers._m0[_64 + 1u] = _67.y;
    indirect_buffers._m0[_64 + 2u] = _67.z;
    indirect_buffers._m0[_64 + 3u] = _67.w;
}

void main()
{
    PARTICLECOUNTER_OFFSET_ALIVECOUNT = 0u;
    PARTICLECOUNTER_OFFSET_DEADCOUNT = PARTICLECOUNTER_OFFSET_ALIVECOUNT + 4u;
    PARTICLECOUNTER_OFFSET_REALEMITCOUNT = PARTICLECOUNTER_OFFSET_DEADCOUNT + 4u;
    PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION = PARTICLECOUNTER_OFFSET_REALEMITCOUNT + 4u;
    THREADCOUNT_EMIT = 256u;
    THREADCOUNT_SIMULATION = 256u;
    COMPUTE_IAB_OFFSET_DISPATCHEMIT = 0u;
    COMPUTE_IAB_OFFSET_DISPATCHSIMULATION = COMPUTE_IAB_OFFSET_DISPATCHEMIT + 12u;
    DRAW_IAB_OFFSET_DRAWPARTICLES = 0u;
    uvec3 param_var_DTid;
    src_cs_main(param_var_DTid);
}

