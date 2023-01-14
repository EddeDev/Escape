#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 3) out flat float v_TexIndex;

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
	Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	Output.TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}