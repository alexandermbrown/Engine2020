#version 430

struct PS_IN
{
    vec4 position;
    vec2 texcoord;
    vec4 color;
    int texindex;
};

uniform sampler2D SPIRV_Cross_Combinedu_Texture0u_Sampler0;
uniform sampler2D SPIRV_Cross_Combinedu_Texture1u_Sampler1;
uniform sampler2D SPIRV_Cross_Combinedu_Texture2u_Sampler2;
uniform sampler2D SPIRV_Cross_Combinedu_Texture3u_Sampler3;
uniform sampler2D SPIRV_Cross_Combinedu_Texture4u_Sampler4;
uniform sampler2D SPIRV_Cross_Combinedu_Texture5u_Sampler5;
uniform sampler2D SPIRV_Cross_Combinedu_Texture6u_Sampler6;
uniform sampler2D SPIRV_Cross_Combinedu_Texture7u_Sampler7;

layout(location = 0) in vec2 in_var_TEXCOORD;
layout(location = 1) in vec4 in_var_COLOR;
layout(location = 2) flat in int in_var_TEXINDEX;
layout(location = 0) out vec4 out_var_SV_TARGET;

vec4 src_ps_main(PS_IN _input)
{
    vec4 tex_color = _input.color;
    switch (_input.texindex)
    {
        case 0:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture0u_Sampler0, _input.texcoord);
            break;
        }
        case 1:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture1u_Sampler1, _input.texcoord);
            break;
        }
        case 2:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture2u_Sampler2, _input.texcoord);
            break;
        }
        case 3:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture3u_Sampler3, _input.texcoord);
            break;
        }
        case 4:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture4u_Sampler4, _input.texcoord);
            break;
        }
        case 5:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture5u_Sampler5, _input.texcoord);
            break;
        }
        case 6:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture6u_Sampler6, _input.texcoord);
            break;
        }
        case 7:
        {
            tex_color *= texture(SPIRV_Cross_Combinedu_Texture7u_Sampler7, _input.texcoord);
            break;
        }
    }
    return tex_color;
}

void main()
{
    PS_IN param_var_input = PS_IN(gl_FragCoord, in_var_TEXCOORD, in_var_COLOR, in_var_TEXINDEX);
    out_var_SV_TARGET = src_ps_main(param_var_input);
}

