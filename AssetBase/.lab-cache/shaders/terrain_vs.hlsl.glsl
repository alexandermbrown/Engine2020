#version 430

out gl_PerVertex
{
    vec4 gl_Position;
};

struct VS_IN_STATIC
{
    vec2 position;
    vec2 texcoord;
};

struct VS_IN_DYNAMIC
{
    vec3 alphavalues;
};

struct PS_IN
{
    vec4 position;
    vec2 texcoord;
    vec3 alphavalues;
};

layout(binding = 1, std140) uniform type_ViewProjCB
{
    layout(row_major) mat4 u_ViewProj;
} ViewProjCB;

layout(binding = 2, std140) uniform type_TransformCB
{
    layout(row_major) mat4 u_Transform;
} TransformCB;

layout(location = 0) in vec2 in_var_POSITION;
layout(location = 1) in vec2 in_var_TEXCOORD;
layout(location = 2) in vec3 in_var_ALPHAVALUES;
layout(location = 0) out vec2 out_var_TEXCOORD;
layout(location = 1) out vec3 out_var_ALPHAVALUES;

PS_IN src_vs_main(VS_IN_STATIC s_input, VS_IN_DYNAMIC d_input)
{
    PS_IN _output;
    _output.position = (vec4(s_input.position, 0.0, 1.0) * TransformCB.u_Transform) * ViewProjCB.u_ViewProj;
    _output.texcoord = s_input.texcoord;
    _output.alphavalues = d_input.alphavalues;
    return _output;
}

void main()
{
    VS_IN_STATIC param_var_s_input = VS_IN_STATIC(in_var_POSITION, in_var_TEXCOORD);
    VS_IN_DYNAMIC param_var_d_input = VS_IN_DYNAMIC(in_var_ALPHAVALUES);
    PS_IN _45 = src_vs_main(param_var_s_input, param_var_d_input);
    gl_Position = _45.position;
    out_var_TEXCOORD = _45.texcoord;
    out_var_ALPHAVALUES = _45.alphavalues;
}

