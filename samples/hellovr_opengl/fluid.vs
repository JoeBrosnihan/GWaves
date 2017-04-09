#version 410
uniform float time;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvCoord;

out vec2 texCoord;

void main()
{
	texCoord = uvCoord;
	gl_Position = vec4(position, 1.0);
}