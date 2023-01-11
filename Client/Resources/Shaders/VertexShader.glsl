#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) out VertexOutput Output;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_InverseViewProjectionMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

void main()
{
	Output.Color = a_Color;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}