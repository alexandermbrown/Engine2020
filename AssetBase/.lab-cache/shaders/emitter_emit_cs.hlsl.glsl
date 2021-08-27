#version 430
layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle
{
    vec4 color;
    vec3 position;
    float rotation;
    vec3 scale;
    float life_left;
    vec3 velocity;
    float start_life;
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
    float u_EmitterPad;
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

float rand(inout float seed, vec2 uv)
{
    float result = fract(sin(seed * dot(uv, vec2(12.98980045318603515625, 78.233001708984375))) * 43758.546875);
    seed += 1.0;
    return result;
}

void src_cs_main(uvec3 thread_id)
{
    uint emit_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_REALEMITCOUNT >> 2u];
    if (thread_id.x < emit_count)
    {
        vec2 uv = vec2(FrameCB.u_RunTime + EmitterCB.u_EmitterRandomness, float(thread_id.x) / 256.0);
        float seed = 0.382239997386932373046875;
        Particle particle;
        particle.position = vec3(0.0);
        particle.rotation = 0.0;
        particle.scale = EmitterCB.u_Scale * EmitterCB.u_ScaleGraph[0].y;
        vec2 param_var_uv = uv;
        float _157 = rand(seed, param_var_uv);
        float speed = (_157 * (EmitterCB.u_SpeedRange.y - EmitterCB.u_SpeedRange.x)) + EmitterCB.u_SpeedRange.x;
        vec2 param_var_uv_1 = uv;
        float _173 = rand(seed, param_var_uv_1);
        float angle = (_173 * 2.0) * 3.1415927410125732421875;
        particle.velocity = vec3(speed * cos(angle), speed * sin(angle), 0.0);
        vec2 param_var_uv_2 = uv;
        float _187 = rand(seed, param_var_uv_2);
        particle.start_life = (_187 * (EmitterCB.u_LifeSpan.y - EmitterCB.u_LifeSpan.x)) + EmitterCB.u_LifeSpan.x;
        particle.life_left = particle.start_life;
        vec2 param_var_uv_3 = uv;
        float _205 = rand(seed, param_var_uv_3);
        vec2 param_var_uv_4 = uv;
        float _209 = rand(seed, param_var_uv_4);
        vec2 param_var_uv_5 = uv;
        float _213 = rand(seed, param_var_uv_5);
        particle.color = vec4((_205 * 0.5) + 0.4000000059604644775390625, (_209 * 0.5) + 0.4000000059604644775390625, (_213 * 0.5) + 0.4000000059604644775390625, EmitterCB.u_AlphaGraph[0].y);
        uint _226 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_DEADCOUNT >> 2u], 4294967295u);
        uint dead_count = _226;
        uint new_particle_index = dead_buffer_1._m0[dead_count - 1u];
        particles._m0[new_particle_index] = Particle(particle.color, particle.position, particle.rotation, particle.scale, particle.life_left, particle.velocity, particle.start_life);
        uint _246 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT >> 2u], 1u);
        uint alive_count = _246;
        alive_buffer_current._m0[alive_count] = new_particle_index;
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

