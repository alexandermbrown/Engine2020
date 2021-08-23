#version 430

out gl_PerVertex
{
    vec4 gl_Position;
};

struct VS_IN
{
    vec3 position;
    vec2 texcoord;
    float texindex;
};

struct PS_IN
{
    vec4 position;
    vec2 texcoord;
    int texindex;
};

layout(binding = 1, std140) uniform type_ViewProjCB
{
    layout(row_major) mat4 u_ViewProj;
} ViewProjCB;

layout(binding = 2, std140) uniform type_TransformCB
{
    layout(row_major) mat4 u_Transform;
} TransformCB;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec2 in_var_TEXCOORD;
layout(location = 2) in float in_var_TEXINDEX;
layout(location = 0) out vec2 out_var_TEXCOORD;
layout(location = 1) flat out int out_var_TEXINDEX;

PS_IN src_vs_main(VS_IN _input)
{
    PS_IN _output;
    _output.position = (vec4(_input.position, 1.0) * TransformCB.u_Transform) * ViewProjCB.u_ViewProj;
    _output.texcoord = _input.texcoord;
    _output.texindex = int(_input.texindex);
    return _output;
}

void main()
{
    VS_IN param_var_input = VS_IN(in_var_POSITION, in_var_TEXCOORD, in_var_TEXINDEX);
    PS_IN _41 = src_vs_main(param_var_input);
    gl_Position = _41.position;
    out_var_TEXCOORD = _41.texcoord;
    out_var_TEXINDEX = _41.texindex;
}

