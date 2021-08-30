#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(binding = 8, std140) uniform type_SortCB
{
    ivec3 job_params;
    uint counterReadOffset;
} SortCB;

layout(binding = 0, std430) readonly buffer type_ByteAddressBuffer
{
    uint _m0[];
} counterBuffer;

layout(binding = 3, std430) buffer type_RWByteAddressBuffer
{
    uint _m0[];
} indirectBuffers;

void src_cs_main(uvec3 DTid)
{
    uint itemCount = counterBuffer._m0[SortCB.counterReadOffset >> 2u];
    if (itemCount > 0u)
    {
        uint threadCount = ((itemCount - 1u) >> (9u & 31u)) + 1u;
        uint _56 = 0u >> 2u;
        uvec3 _58 = uvec3(threadCount, 1u, 1u);
        indirectBuffers._m0[_56] = _58.x;
        indirectBuffers._m0[_56 + 1u] = _58.y;
        indirectBuffers._m0[_56 + 2u] = _58.z;
    }
    else
    {
        uint _67 = 0u >> 2u;
        indirectBuffers._m0[_67] = uvec3(0u).x;
        indirectBuffers._m0[_67 + 1u] = uvec3(0u).y;
        indirectBuffers._m0[_67 + 2u] = uvec3(0u).z;
    }
}

void main()
{
    uvec3 param_var_DTid;
    src_cs_main(param_var_DTid);
}

