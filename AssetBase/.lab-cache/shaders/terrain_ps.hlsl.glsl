#version 430

struct PS_IN
{
    vec4 position;
    vec2 texcoord;
    vec3 alphavalues;
};

layout(binding = 5, std140) uniform type_AtlasBoundsCB
{
    vec4 u_AtlasBounds0;
    vec4 u_AtlasBounds1;
    vec4 u_AtlasBounds2;
    vec4 u_AtlasBounds3;
    vec3 u_NoiseWeights;
    vec3 u_BlendWidths;
} AtlasBoundsCB;

uniform sampler2D SPIRV_Cross_Combinedu_Textureu_TextureSampler;
uniform sampler2D SPIRV_Cross_Combinedu_Noise1u_Noise1Sampler;
uniform sampler2D SPIRV_Cross_Combinedu_Noise2u_Noise2Sampler;
uniform sampler2D SPIRV_Cross_Combinedu_Noise3u_Noise3Sampler;

layout(location = 0) in vec2 in_var_TEXCOORD;
layout(location = 1) in vec3 in_var_ALPHAVALUES;
layout(location = 0) out vec4 out_var_SV_TARGET;

float width_blend(float alpha, float blendwidth)
{
    if (blendwidth == 0.0)
    {
        if (alpha < 0.5)
        {
            return 0.0;
        }
        else
        {
            return 1.0;
        }
    }
    else
    {
        return clamp(((1.0 / blendwidth) * (alpha - 0.5)) + 0.5, 0.0, 1.0);
    }
    return 0.0;
}

vec4 src_ps_main(PS_IN _input)
{
    vec3 terraincolor = texture(SPIRV_Cross_Combinedu_Textureu_TextureSampler, vec2(AtlasBoundsCB.u_AtlasBounds0.x + (AtlasBoundsCB.u_AtlasBounds0.z * _input.texcoord.x), AtlasBoundsCB.u_AtlasBounds0.y + (AtlasBoundsCB.u_AtlasBounds0.w * _input.texcoord.y))).xyz;
    float alphaoffset1 = (texture(SPIRV_Cross_Combinedu_Noise1u_Noise1Sampler, _input.texcoord).x - 0.5) * AtlasBoundsCB.u_NoiseWeights.x;
    float param_var_alpha = _input.alphavalues.x + alphaoffset1;
    float param_var_blendwidth = AtlasBoundsCB.u_BlendWidths.x;
    float alpha1 = width_blend(param_var_alpha, param_var_blendwidth);
    float alphaoffset2 = (texture(SPIRV_Cross_Combinedu_Noise2u_Noise2Sampler, _input.texcoord).x - 0.5) * AtlasBoundsCB.u_NoiseWeights.y;
    float param_var_alpha_1 = _input.alphavalues.y + alphaoffset2;
    float param_var_blendwidth_1 = AtlasBoundsCB.u_BlendWidths.y;
    float alpha2 = width_blend(param_var_alpha_1, param_var_blendwidth_1);
    float alphaoffset3 = (texture(SPIRV_Cross_Combinedu_Noise3u_Noise3Sampler, _input.texcoord).x - 0.5) * AtlasBoundsCB.u_NoiseWeights.z;
    float param_var_alpha_2 = _input.alphavalues.z + alphaoffset3;
    float param_var_blendwidth_2 = AtlasBoundsCB.u_BlendWidths.z;
    float alpha3 = width_blend(param_var_alpha_2, param_var_blendwidth_2);
    if (alpha1 > 0.0)
    {
        terraincolor = mix(terraincolor, texture(SPIRV_Cross_Combinedu_Textureu_TextureSampler, vec2(AtlasBoundsCB.u_AtlasBounds1.x + (AtlasBoundsCB.u_AtlasBounds1.z * _input.texcoord.x), AtlasBoundsCB.u_AtlasBounds1.y + (AtlasBoundsCB.u_AtlasBounds1.w * _input.texcoord.y))).xyz, vec3(alpha1));
    }
    if (alpha2 > 0.0)
    {
        terraincolor = mix(terraincolor, texture(SPIRV_Cross_Combinedu_Textureu_TextureSampler, vec2(AtlasBoundsCB.u_AtlasBounds2.x + (AtlasBoundsCB.u_AtlasBounds2.z * _input.texcoord.x), AtlasBoundsCB.u_AtlasBounds2.y + (AtlasBoundsCB.u_AtlasBounds2.w * _input.texcoord.y))).xyz, vec3(alpha2));
    }
    if (alpha3 > 0.0)
    {
        terraincolor = mix(terraincolor, texture(SPIRV_Cross_Combinedu_Textureu_TextureSampler, vec2(AtlasBoundsCB.u_AtlasBounds3.x + (AtlasBoundsCB.u_AtlasBounds3.z * _input.texcoord.x), AtlasBoundsCB.u_AtlasBounds3.y + (AtlasBoundsCB.u_AtlasBounds3.w * _input.texcoord.y))).xyz, vec3(alpha3));
    }
    return vec4(terraincolor, 1.0);
}

void main()
{
    PS_IN param_var_input = PS_IN(gl_FragCoord, in_var_TEXCOORD, in_var_ALPHAVALUES);
    out_var_SV_TARGET = src_ps_main(param_var_input);
}

