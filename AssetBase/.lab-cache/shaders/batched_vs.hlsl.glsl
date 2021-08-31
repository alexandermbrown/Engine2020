#version 430

out gl_PerVertex
{
    vec4 gl_Position;
};

struct VERTEX_IN
{
    vec3 position;
    float texindex;
    vec2 texcoord;
    vec4 color;
};

struct PS_IN
{
    vec4 position;
    vec2 texcoord;
    vec4 color;
    int texindex;
};

layout(binding = 1, std140) uniform type_CameraCB
{
    layout(row_major) mat4 u_View;
    layout(row_major) mat4 u_Proj;
    layout(row_major) mat4 u_ViewProj;
} CameraCB;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in float in_var_TEXINDEX;
layout(location = 2) in vec2 in_var_TEXCOORD;
layout(location = 3) in vec4 in_var_COLOR;
layout(location = 0) out vec2 out_var_TEXCOORD;
layout(location = 1) out vec4 out_var_COLOR;
layout(location = 2) flat out int out_var_TEXINDEX;

PS_IN src_vs_main(VERTEX_IN _input)
{
    PS_IN _output;
    _output.position = vec4(_input.position, 1.0) * CameraCB.u_ViewProj;
    _output.texcoord = _input.texcoord;
    _output.color = _input.color;
    _output.texindex = int(_input.texindex);
    return _output;
}

void main()
{
    VERTEX_IN param_var_input = VERTEX_IN(in_var_POSITION, in_var_TEXINDEX, in_var_TEXCOORD, in_var_COLOR);
    PS_IN _43 = src_vs_main(param_var_input);
    gl_Position = _43.position;
    out_var_TEXCOORD = _43.texcoord;
    out_var_COLOR = _43.color;
    out_var_TEXINDEX = _43.texindex;
}

