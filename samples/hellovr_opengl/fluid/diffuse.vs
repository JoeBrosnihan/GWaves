#version 410

uniform float time;
uniform mat4 mat_model;
uniform mat4 mat_viewproj;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec2 texCoord;

void main()
{
	texCoord = uvCoord;
	gl_Position = mat_viewproj * (mat_model * vec4(position, 1.0));
}