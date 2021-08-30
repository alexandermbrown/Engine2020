#version 430

struct PS_IN
{
    vec4 position;
    vec2 texcoord;
    vec4 color;
};

uniform sampler2D SPIRV_Cross_Combinedu_Textureu_Sampler;

layout(location = 0) in vec2 in_var_TEXCOORD;
layout(location = 1) in vec4 in_var_COLOR;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 src_ps_main(PS_IN _input)
{
    return _input.color * texture(SPIRV_Cross_Combinedu_Textureu_Sampler, _input.texcoord);
}

void main()
{
    PS_IN param_var_input = PS_IN(gl_FragCoord, in_var_TEXCOORD, in_var_COLOR);
    out_var_SV_TARGET = src_ps_main(param_var_input);
}

