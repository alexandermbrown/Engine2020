#version 430

out gl_PerVertex
{
    vec4 gl_Position;
};

struct VS_IN
{
    vec3 position;
    vec4 color;
};

struct PS_IN
{
    vec4 position;
    vec4 color;
};

layout(binding = 1, std140) uniform type_CameraCB
{
    layout(row_major) mat4 u_View;
    layout(row_major) mat4 u_Proj;
    layout(row_major) mat4 u_ViewProj;
} CameraCB;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec4 in_var_COLOR;
layout(location = 0) out vec4 out_var_COLOR;

PS_IN src_vs_main(VS_IN _input)
{
    PS_IN _output;
    _output.position = vec4(_input.position, 1.0) * CameraCB.u_ViewProj;
    _output.color = _input.color;
    return _output;
}

void main()
{
    VS_IN param_var_input = VS_IN(in_var_POSITION, in_var_COLOR);
    PS_IN _31 = src_vs_main(param_var_input);
    gl_Position = _31.position;
    out_var_COLOR = _31.color;
}

