#version 430

struct PS_IN
{
    vec4 position;
};

layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 src_ps_main(PS_IN _input)
{
    return vec4(0.800000011920928955078125, 1.0, 1.0, 1.0);
}

void main()
{
    PS_IN param_var_input;
    out_var_SV_TARGET = src_ps_main(param_var_input);
}

