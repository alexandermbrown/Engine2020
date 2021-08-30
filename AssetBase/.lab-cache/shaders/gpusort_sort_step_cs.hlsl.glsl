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

void src_cs_main(uvec3 Gid, uvec3 GTid)
{
    uint NumElements = counterBuffer._m0[SortCB.counterReadOffset >> 2u];
    uvec4 tgp;
    tgp.x = Gid.x * 256u;
    tgp.y = 0u;
    tgp.z = NumElements;
    tgp.w = min(512u, max(0u, (NumElements - (Gid.x * 512u))));
    uint localID = tgp.x + GTid.x;
    uint index_low = localID & uint(SortCB.job_params.x - 1);
    uint index_high = 2u * (localID - index_low);
    uint index = (tgp.y + index_high) + index_low;
    uint nSwapElem = ((tgp.y + index_high) + uint(SortCB.job_params.y)) + (uint(SortCB.job_params.z) * index_low);
    if (nSwapElem < (tgp.y + tgp.z))
    {
        uint index_a = indexBuffer._m0[index];
        uint index_b = indexBuffer._m0[nSwapElem];
        float a = comparisonBuffer._m0[index_a];
        float b = comparisonBuffer._m0[index_b];
        if (a > b)
        {
            indexBuffer._m0[index] = index_b;
            indexBuffer._m0[nSwapElem] = index_a;
        }
    }
}

void main()
{
    uvec3 param_var_Gid = gl_WorkGroupID;
    uvec3 param_var_GTid = gl_LocalInvocationID;
    src_cs_main(param_var_Gid, param_var_GTid);
}

