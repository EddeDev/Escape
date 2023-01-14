#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat float v_TexIndex;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 color = Input.Color;
	color *= texture(u_Textures[int(v_TexIndex)], Input.TexCoord * Input.TilingFactor);
	if (color.a == 0.0)
		discard;
	o_Color = color;
}