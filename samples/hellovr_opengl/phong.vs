#version 410

uniform mat4 mat_model;
uniform mat4 mat_viewproj;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec2 texCoord;
out vec3 worldPos;
out vec3 worldNormal;

void main()
{
	texCoord = uvCoord;
	worldPos = (mat_model * vec4(position, 1.0)).xyz;
	worldNormal = (mat_model * vec4(normal, 0.0)).xyz;

	gl_Position = mat_viewproj * vec4(worldPos, 1.0);
}