#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(binding = 4, std140) uniform type_EmitterCB
{
    layout(row_major) mat4 u_EmitterTransform;
    vec2 u_LifeSpan;
    uint u_EmitCount;
    float u_EmitterRandomness;
    vec2 u_SpeedRange;
    vec3 u_Scale;
    uint u_RelativeToWorld;
    vec4 u_Rotation;
    vec3 u_SpawnVolume;
    float u_EmitterPad1;
    vec4 u_ScaleGraph[8];
    vec4 u_AlphaGraph[8];
} EmitterCB;

layout(binding = 4, std430) buffer type_RWByteAddressBuffer
{
    uint _m0[];
} counter_buffer;

layout(binding = 6, std430) buffer draw_iab
{
    uint _m0[];
} draw_iab_1;

uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;

void src_cs_main(uvec3 DTid)
{
    uint particle_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION >> 2u];
    uint _66 = DRAW_IAB_OFFSET_DRAWPARTICLES >> 2u;
    uvec4 _70 = uvec4(6u * particle_count, 1u, 0u, 0u);
    draw_iab_1._m0[_66] = _70.x;
    draw_iab_1._m0[_66 + 1u] = _70.y;
    draw_iab_1._m0[_66 + 2u] = _70.z;
    draw_iab_1._m0[_66 + 3u] = _70.w;
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
    uvec3 param_var_DTid;
    src_cs_main(param_var_DTid);
}

