#version 430
layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

layout(binding = 8, std140) uniform type_SortCB
{
    ivec3 job_params;
    uint counterReadOffset;
} SortCB;

layout(binding = 0, std430) readonly buffer type_ByteAddressBuffer
{
    uint _m0[];
} counterBuffer;

layout(binding = 1, std430) readonly buffer type_StructuredBuffer_float
{
    float _m0[];
} comparisonBuffer;

layout(binding = 2, std430) buffer type_RWStructuredBuffer_uint
{
    uint _m0[];
} indexBuffer;

layout(binding = 3, std430) buffer type_ACSBuffer_counter
{
    int counter;
} counter_var_indexBuffer;

shared vec2 g_LDS[512];

void src_cs_main(uvec3 Gid, uvec3 DTid, uvec3 GTid, uint GI)
{
    uint NumElements = counterBuffer._m0[SortCB.counterReadOffset >> 2u];
    uvec4 tgp;
    tgp.x = Gid.x * 256u;
    tgp.y = 0u;
    tgp.z = NumElements;
    tgp.w = min(512u, max(0u, (NumElements - (Gid.x * 512u))));
    uint GlobalBaseIndex = (tgp.y + (tgp.x * 2u)) + GTid.x;
    uint LocalBaseIndex = GI;
    uint i = 0u;
    for (; i < 2u; i++)
    {
        if ((GI + (i * 256u)) < tgp.w)
        {
            uint particleIndex = indexBuffer._m0[GlobalBaseIndex + (i * 256u)];
            float dist = comparisonBuffer._m0[particleIndex];
            g_LDS[LocalBaseIndex + (i * 256u)] = vec2(dist, float(particleIndex));
        }
    }
    barrier();
    for (int nMergeSubSize = 256; nMergeSubSize > 0; nMergeSubSize = nMergeSubSize >> (1 & 31))
    {
        int tmp_index = int(GI);
        int index_low = tmp_index & (nMergeSubSize - 1);
        int index_high = 2 * (tmp_index - index_low);
        int index = index_high + index_low;
        uint nSwapElem = uint((index_high + nMergeSubSize) + index_low);
        if (nSwapElem < tgp.w)
        {
            vec2 a = g_LDS[index];
            vec2 b = g_LDS[nSwapElem];
            if (a.x > b.x)
            {
                g_LDS[index] = b;
                g_LDS[nSwapElem] = a;
            }
        }
        barrier();
    }
    i = 0u;
    for (; i < 2u; i++)
    {
        if ((GI + (i * 256u)) < tgp.w)
        {
            indexBuffer._m0[GlobalBaseIndex + (i * 256u)] = uint(g_LDS[LocalBaseIndex + (i * 256u)].y);
        }
    }
}

void main()
{
    uvec3 param_var_Gid = gl_WorkGroupID;
    uvec3 param_var_GTid = gl_LocalInvocationID;
    uint param_var_GI = gl_LocalInvocationIndex;
    uvec3 param_var_DTid;
    src_cs_main(param_var_Gid, param_var_DTid, param_var_GTid, param_var_GI);
}

