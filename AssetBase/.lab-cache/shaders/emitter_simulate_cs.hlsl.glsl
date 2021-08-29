#version 430
layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle
{
    vec4 color;
    vec3 position;
    float angle;
    vec3 scale;
    float life_left;
    vec3 velocity;
    float start_life;
    float angular_velocity;
    float _pad0;
    float _pad1;
    float _pad2;
};

layout(binding = 0, std140) uniform type_FrameCB
{
    float u_RunTime;
    float u_DeltaTime;
} FrameCB;

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
    vec3 u_EmitVolume;
    float u_EmitterPad0;
    vec3 u_Acceleration;
    float u_EmitterPad1;
    vec4 u_ScaleGraph[8];
    vec4 u_AlphaGraph[8];
} EmitterCB;

layout(binding = 0, std430) buffer type_RWStructuredBuffer_Particle
{
    Particle _m0[];
} particles;

layout(binding = 5, std430) buffer type_ACSBuffer_counter
{
    int counter;
} counter_var_particles;

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

uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;

float graph_lerp(vec4 val_vs_life[8], float life)
{
    for (int i = 1; i < 8; i++)
    {
        if (life <= val_vs_life[i].x)
        {
            return mix(val_vs_life[i - 1].y, val_vs_life[i].y, (life - val_vs_life[i - 1].x) / (val_vs_life[i].x - val_vs_life[i - 1].x));
        }
    }
    return val_vs_life[8 - 1].y;
}

void src_cs_main(uvec3 thread_id)
{
    uint alive_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT >> 2u];
    if (thread_id.x < alive_count)
    {
        uint particle_index = alive_buffer_current._m0[thread_id.x];
        Particle particle = Particle(particles._m0[particle_index].color, particles._m0[particle_index].position, particles._m0[particle_index].angle, particles._m0[particle_index].scale, particles._m0[particle_index].life_left, particles._m0[particle_index].velocity, particles._m0[particle_index].start_life, particles._m0[particle_index].angular_velocity, particles._m0[particle_index]._pad0, particles._m0[particle_index]._pad1, particles._m0[particle_index]._pad2);
        if (particle.life_left > 0.0)
        {
            particle.velocity += (EmitterCB.u_Acceleration * FrameCB.u_DeltaTime);
            particle.position += (particle.velocity * FrameCB.u_DeltaTime);
            particle.angle += (particle.angular_velocity * FrameCB.u_DeltaTime);
            particle.life_left -= FrameCB.u_DeltaTime;
            float life_fraction = clamp(1.0 - (particle.life_left / particle.start_life), 0.0, 1.0);
            vec4 param_var_val_vs_life[8] = vec4[](EmitterCB.u_ScaleGraph[0], EmitterCB.u_ScaleGraph[1], EmitterCB.u_ScaleGraph[2], EmitterCB.u_ScaleGraph[3], EmitterCB.u_ScaleGraph[4], EmitterCB.u_ScaleGraph[5], EmitterCB.u_ScaleGraph[6], EmitterCB.u_ScaleGraph[7]);
            float param_var_life = life_fraction;
            particle.scale = EmitterCB.u_Scale * graph_lerp(param_var_val_vs_life, param_var_life);
            vec4 param_var_val_vs_life_1[8] = vec4[](EmitterCB.u_AlphaGraph[0], EmitterCB.u_AlphaGraph[1], EmitterCB.u_AlphaGraph[2], EmitterCB.u_AlphaGraph[3], EmitterCB.u_AlphaGraph[4], EmitterCB.u_AlphaGraph[5], EmitterCB.u_AlphaGraph[6], EmitterCB.u_AlphaGraph[7]);
            float param_var_life_1 = life_fraction;
            particle.color.w = graph_lerp(param_var_val_vs_life_1, param_var_life_1);
            particles._m0[particle_index] = Particle(particle.color, particle.position, particle.angle, particle.scale, particle.life_left, particle.velocity, particle.start_life, particle.angular_velocity, particle._pad0, particle._pad1, particle._pad2);
            uint _239 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION >> 2u], 1u);
            uint new_alive_index = _239;
            alive_buffer_new_1._m0[new_alive_index] = particle_index;
        }
        else
        {
            uint _246 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_DEADCOUNT >> 2u], 1u);
            uint dead_index = _246;
            dead_buffer_1._m0[dead_index] = particle_index;
        }
    }
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
    uvec3 param_var_thread_id = gl_GlobalInvocationID;
    src_cs_main(param_var_thread_id);
}

