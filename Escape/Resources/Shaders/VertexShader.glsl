#version 450 core

layout(location = 0) in vec3 a_Position;

struct Constants
{
	mat4 Transform;
};

layout(location = 0) uniform Constants u_Constants;

void main()
{
	gl_Position = u_Constants.Transform * vec4(a_Position, 1.0);
}