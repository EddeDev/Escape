#version 450 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_InverseViewProjectionMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

struct Constants
{
	mat4 Transform;
};

layout(location = 0) uniform Constants u_Constants;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_Constants.Transform * vec4(a_Position, 1.0);
}