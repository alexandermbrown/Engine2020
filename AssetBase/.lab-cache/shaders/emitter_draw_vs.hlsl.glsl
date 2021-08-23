#version 430
#ifdef GL_ARB_shader_draw_parameters
#extension GL_ARB_shader_draw_parameters : enable
#endif

out gl_PerVertex
{
    vec4 gl_Position;
};

struct Particle
{
    vec3 position;
    float max_life;
    vec3 velocity;
    float life;
};

struct PS_IN
{
    vec4 position;
};

layout(binding = 1, std140) uniform type_ViewProjCB
{
    layout(row_major) mat4 u_ViewProj;
} ViewProjCB;

layout(binding = 4, std140) uniform type_EmitterCB
{
    layout(row_major) mat4 u_EmitterTransform;
    uint u_EmitCount;
    float u_EmitterRandomness;
    vec2 u_ParticleLifeSpan;
} EmitterCB;

layout(binding = 1, std430) readonly buffer type_StructuredBuffer_Particle
{
    Particle _m0[];
} particle_buffer;

layout(binding = 2, std430) readonly buffer type_StructuredBuffer_uint
{
    uint _m0[];
} alive_list;

#ifdef GL_ARB_shader_draw_parameters
#define SPIRV_Cross_BaseInstance gl_BaseInstanceARB
#else
uniform int SPIRV_Cross_BaseInstance;
#endif
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;
vec3 BILLBOARD[4];

PS_IN src_vs_main(uint vertex_id, uint instance_id)
{
    Particle particle = Particle(particle_buffer._m0[alive_list._m0[instance_id]].position, particle_buffer._m0[alive_list._m0[instance_id]].max_life, particle_buffer._m0[alive_list._m0[instance_id]].velocity, particle_buffer._m0[alive_list._m0[instance_id]].life);
    vec3 quad_pos = BILLBOARD[vertex_id];
    PS_IN _output;
    _output.position = vec4(particle.position, 1.0);
    vec3 _119 = _output.position.xyz + quad_pos;
    _output.position = vec4(_119.x, _119.y, _119.z, _output.position.w);
    _output.position *= ViewProjCB.u_ViewProj;
    return _output;
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
    BILLBOARD = vec3[](vec3(-1.0, -1.0, 0.0), vec3(1.0, -1.0, 0.0), vec3(-1.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0));
    uint param_var_vertex_id = uint(gl_VertexID);
    uint SV_InstanceID = uint((gl_InstanceID + SPIRV_Cross_BaseInstance)) - uint(SPIRV_Cross_BaseInstance);
    uint param_var_instance_id = SV_InstanceID;
    gl_Position = src_vs_main(param_var_vertex_id, param_var_instance_id).position;
}

