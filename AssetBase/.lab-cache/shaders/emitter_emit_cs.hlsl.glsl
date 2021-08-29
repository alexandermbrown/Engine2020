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

float rand(inout float seed, vec2 uv)
{
    float result = fract(sin(seed * dot(uv, vec2(12.98980045318603515625, 78.233001708984375))) * 43758.546875);
    seed += 1.0;
    return result;
}

float rand_in_range(inout float seed, vec2 uv, float minimum, float maximum)
{
    vec2 param_var_uv = uv;
    float _338 = rand(seed, param_var_uv);
    return (_338 * (maximum - minimum)) + minimum;
}

void src_cs_main(uvec3 thread_id)
{
    uint emit_count = counter_buffer._m0[PARTICLECOUNTER_OFFSET_REALEMITCOUNT >> 2u];
    if (thread_id.x < emit_count)
    {
        vec2 uv = vec2(FrameCB.u_RunTime + EmitterCB.u_EmitterRandomness, float(thread_id.x) / 256.0);
        float seed = 0.382239997386932373046875;
        vec3 half_volume = EmitterCB.u_EmitVolume / vec3(2.0);
        vec2 param_var_uv = uv;
        float param_var_minimum = -half_volume.x;
        float param_var_maximum = half_volume.x;
        float _177 = rand_in_range(seed, param_var_uv, param_var_minimum, param_var_maximum);
        vec2 param_var_uv_1 = uv;
        float param_var_minimum_1 = -half_volume.y;
        float param_var_maximum_1 = half_volume.y;
        float _185 = rand_in_range(seed, param_var_uv_1, param_var_minimum_1, param_var_maximum_1);
        vec2 param_var_uv_2 = uv;
        float param_var_minimum_2 = -half_volume.z;
        float param_var_maximum_2 = half_volume.z;
        float _192 = rand_in_range(seed, param_var_uv_2, param_var_minimum_2, param_var_maximum_2);
        vec3 emit_offset = vec3(_177, _185, _192);
        Particle particle;
        if (EmitterCB.u_RelativeToWorld != 0u)
        {
            particle.position = (vec4(emit_offset, 1.0) * EmitterCB.u_EmitterTransform).xyz;
        }
        else
        {
            particle.position = emit_offset;
        }
        vec2 param_var_uv_3 = uv;
        float param_var_minimum_3 = EmitterCB.u_Rotation.x;
        float param_var_maximum_3 = EmitterCB.u_Rotation.y;
        float _221 = rand_in_range(seed, param_var_uv_3, param_var_minimum_3, param_var_maximum_3);
        particle.angle = _221;
        particle.scale = EmitterCB.u_Scale * EmitterCB.u_ScaleGraph[0].y;
        vec2 param_var_uv_4 = uv;
        float param_var_minimum_4 = EmitterCB.u_SpeedRange.x;
        float param_var_maximum_4 = EmitterCB.u_SpeedRange.y;
        float _240 = rand_in_range(seed, param_var_uv_4, param_var_minimum_4, param_var_maximum_4);
        float speed = _240;
        vec2 param_var_uv_5 = uv;
        float _242 = rand(seed, param_var_uv_5);
        float angle = (_242 * 2.0) * 3.1415927410125732421875;
        particle.velocity = vec3(speed * cos(angle), speed * sin(angle), 0.0);
        vec2 param_var_uv_6 = uv;
        float param_var_minimum_5 = EmitterCB.u_Rotation.z;
        float param_var_maximum_5 = EmitterCB.u_Rotation.w;
        float _263 = rand_in_range(seed, param_var_uv_6, param_var_minimum_5, param_var_maximum_5);
        particle.angular_velocity = _263;
        vec2 param_var_uv_7 = uv;
        float param_var_minimum_6 = EmitterCB.u_LifeSpan.x;
        float param_var_maximum_6 = EmitterCB.u_LifeSpan.y;
        float _272 = rand_in_range(seed, param_var_uv_7, param_var_minimum_6, param_var_maximum_6);
        particle.start_life = _272;
        particle.life_left = particle.start_life;
        vec2 param_var_uv_8 = uv;
        float _278 = rand(seed, param_var_uv_8);
        vec2 param_var_uv_9 = uv;
        float _282 = rand(seed, param_var_uv_9);
        vec2 param_var_uv_10 = uv;
        float _286 = rand(seed, param_var_uv_10);
        particle.color = vec4((_278 * 0.5) + 0.4000000059604644775390625, (_282 * 0.5) + 0.4000000059604644775390625, (_286 * 0.5) + 0.4000000059604644775390625, EmitterCB.u_AlphaGraph[0].y);
        particle._pad0 = 0.0;
        particle._pad1 = 0.0;
        particle._pad2 = 0.0;
        uint _302 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_DEADCOUNT >> 2u], 4294967295u);
        uint dead_count = _302;
        uint new_particle_index = dead_buffer_1._m0[dead_count - 1u];
        particles._m0[new_particle_index] = Particle(particle.color, particle.position, particle.angle, particle.scale, particle.life_left, particle.velocity, particle.start_life, particle.angular_velocity, particle._pad0, particle._pad1, particle._pad2);
        uint _326 = atomicAdd(counter_buffer._m0[PARTICLECOUNTER_OFFSET_ALIVECOUNT >> 2u], 1u);
        uint alive_count = _326;
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

