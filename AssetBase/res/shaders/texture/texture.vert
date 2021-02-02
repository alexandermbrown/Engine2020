#version 420 core

layout(location = 0) in vec2 POSITION;
layout(location = 1) in vec2 TEXCOORD;

layout(std140, binding = 0) uniform ViewProjectionMatrix
{
	mat4 u_ViewProj;
};

layout(std140, binding = 1) uniform TransformMatrix
{
	mat4 u_Transform;
};

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = TEXCOORD;
	gl_Position = u_ViewProj * u_Transform * vec4(POSITION, 0.0, 1.0);
}
