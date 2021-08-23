#version 430
layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle
{
    vec3 position;
    float max_life;
    vec3 velocity;
    float life;
};

layout(binding = 0, std140) uniform type_FrameCB
{
    float u_RunTime;
    float u_DeltaTime;
} FrameCB;

layout(binding = 4, std140) uniform type_EmitterCB
{
    layout(row_major) mat4 u_EmitterTransform;
    uint u_EmitCount;
    float u_EmitterRandomness;
    vec2 u_ParticleLifeSpan;
} EmitterCB;

layout(binding = 0, std430) buffer type_RWStructuredBuffer_Particle
{
    Particle _m0[];
} particle_buffer;

layout(binding = 5, std430) buffer type_ACSBuffer_counter
{
    int counter;
} counter_var_particle_buffer;

layout(binding = 1, std430) buffer type_RWStructuredBuffer_uint
{
    uint _m0[];
} alive_buffer_current;

layout(binding = 6, std430) buffer counter_var_alive_buffer_current
{
    int counter;
} counter_var_alive_buffer_current_1;

layout(binding = 2, std430) buffer alive_buffer_new
{
    uint _m0[];
} alive_buffer_new_1;

layout(binding = 7, std430) buffer counter_var_alive_buffer_new
{
    int counter;
} counter_var_alive_buffer_new_1;

layout(binding = 3, std430) buffer dead_buffer
{
    uint _m0[];
} dead_buffer_1;

layout(binding = 8, std430) buffer counter_var_dead_buffer
{
    int counter;
} counter_var_dead_buffer_1;

layout(binding = 4, std430) buffer type_RWByteAddressBuffer
{
    uint _m0[];
} counter_buffer;

uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;

void src_cs_main(uvec3 thread_id)
{
    uint alive_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT >> 2u];
    if (thread_id.x < alive_count)
    {
        float dt = FrameCB.u_DeltaTime;
        uint particle_index = alive_buffer_current._m0[thread_id.x];
        Particle particle = Particle(particle_buffer._m0[particle_index].position, particle_buffer._m0[particle_index].max_life, particle_buffer._m0[particle_index].velocity, particle_buffer._m0[particle_index].life);
        if (particle.life > 0.0)
        {
            particle.position += (particle.velocity * dt);
            particle.life -= dt;
            particle_buffer._m0[particle_index] = Particle(particle.position, particle.max_life, particle.velocity, particle.life);
            uint _145 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION >> 2u], 1u);
            uint new_alive_index = _145;
            alive_buffer_new_1._m0[new_alive_index] = particle_index;
        }
        else
        {
            uint _152 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_DEADCOUNT >> 2u], 1u);
            uint dead_index = _152;
            dead_buffer_1._m0[dead_index] = particle_index;
        }
    }
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
    uvec3 param_var_thread_id = gl_GlobalInvocationID;
    src_cs_main(param_var_thread_id);
}

