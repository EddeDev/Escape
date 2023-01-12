#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) in VertexOutput Input;

void main()
{
	vec4 color = Input.Color;
	if (color.a == 0.0)
		discard;
	o_Color = color;
}