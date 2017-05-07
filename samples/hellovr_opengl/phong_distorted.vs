#version 410

uniform mat4 mat_model;
uniform mat4 mat_viewproj;
uniform float time;

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

	//This was used to wait before warping for the demo video
	//float weight = .2 * clamp((time - 10) / 5, 0, 1);

	float weight = .2;
	float deformation = 1 + weight * cos(worldPos.x + 2 * time);
	float stretchFactor = 1 / sqrt(deformation);
	
	worldPos = worldPos + vec3(weight * sin(worldPos.x + 2 * time), 0, 0);
	worldPos = worldPos * vec3(1, stretchFactor, stretchFactor);

	worldNormal = (mat_model * vec4(normal, 0.0)).xyz;

	gl_Position = mat_viewproj * vec4(worldPos, 1.0);
}