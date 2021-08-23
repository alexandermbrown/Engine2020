#version 430

struct PS_IN
{
    vec4 position;
    vec4 color;
};

layout(location = 0) in vec4 in_var_COLOR;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 src_ps_main(PS_IN _input)
{
    return _input.color;
}

void main()
{
    PS_IN param_var_input = PS_IN(gl_FragCoord, in_var_COLOR);
    out_var_SV_TARGET = src_ps_main(param_var_input);
}

