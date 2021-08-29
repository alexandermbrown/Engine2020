#version 430

out gl_PerVertex
{
    vec4 gl_Position;
};

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

struct PS_IN
{
    vec4 position;
    vec4 color;
};

layout(binding = 1, std140) uniform type_ViewProjCB
{
    layout(row_major) mat4 u_ViewProj;
} ViewProjCB;

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

layout(binding = 1, std430) readonly buffer type_StructuredBuffer_Particle
{
    Particle _m0[];
} particle_buffer;

layout(binding = 2, std430) readonly buffer type_StructuredBuffer_uint
{
    uint _m0[];
} alive_buffer;

layout(location = 0) out vec4 out_var_COLOR;
uint THREADCOUNT_EMIT;
uint THREADCOUNT_SIMULATION;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT;
uint PARTICLECOUNTER_OFFSET_DEADCOUNT;
uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT;
uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION;
uint COMPUTE_IAB_OFFSET_DISPATCHEMIT;
uint COMPUTE_IAB_OFFSET_DISPATCHSIMULATION;
uint DRAW_IAB_OFFSET_DRAWPARTICLES;
int INDICES[6];
vec3 BILLBOARD[4];

PS_IN src_vs_main(uint vertex_id)
{
    uint _101 = vertex_id / 6u;
    Particle particle = Particle(particle_buffer._m0[alive_buffer._m0[_101]].color, particle_buffer._m0[alive_buffer._m0[_101]].position, particle_buffer._m0[alive_buffer._m0[_101]].angle, particle_buffer._m0[alive_buffer._m0[_101]].scale, particle_buffer._m0[alive_buffer._m0[_101]].life_left, particle_buffer._m0[alive_buffer._m0[_101]].velocity, particle_buffer._m0[alive_buffer._m0[_101]].start_life, particle_buffer._m0[alive_buffer._m0[_101]].angular_velocity, particle_buffer._m0[alive_buffer._m0[_101]]._pad0, particle_buffer._m0[alive_buffer._m0[_101]]._pad1, particle_buffer._m0[alive_buffer._m0[_101]]._pad2);
    vec3 quad_pos = BILLBOARD[INDICES[vertex_id % 6u]];
    quad_pos *= particle.scale;
    float cos_rot = cos(particle.angle);
    float sin_rot = sin(particle.angle);
    vec2 _156 = vec2((quad_pos.x * cos_rot) - (quad_pos.y * sin_rot), (quad_pos.x * sin_rot) + (quad_pos.y * cos_rot));
    quad_pos = vec3(_156.x, _156.y, quad_pos.z);
    PS_IN _output;
    _output.position = vec4(particle.position, 1.0);
    vec3 _171 = _output.position.xyz + quad_pos;
    _output.position = vec4(_171.x, _171.y, _171.z, _output.position.w);
    if (!(EmitterCB.u_RelativeToWorld != 0u))
    {
        _output.position *= EmitterCB.u_EmitterTransform;
    }
    _output.position *= ViewProjCB.u_ViewProj;
    _output.color = particle.color;
    return _output;
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
    INDICES = int[](0, 1, 2, 0, 2, 3);
    BILLBOARD = vec3[](vec3(-0.5, -0.5, 0.0), vec3(0.5, -0.5, 0.0), vec3(0.5, 0.5, 0.0), vec3(-0.5, 0.5, 0.0));
    uint param_var_vertex_id = uint(gl_VertexID);
    PS_IN _83 = src_vs_main(param_var_vertex_id);
    gl_Position = _83.position;
    out_var_COLOR = _83.color;
}

