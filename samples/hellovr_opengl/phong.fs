#version 410 core

uniform vec4 color;

in vec2 texCoord;
in vec3 worldPosition;
in vec3 worldNormal;

out vec4 outputColor;


void main()
{
	vec3 totalLight = vec3(.15, .15, .15);

	float ndot = dot(normalize(worldNormal), vec3(-0.57735, 0.57735, 0.57735));
	ndot = max(ndot, 0.0);
	totalLight = totalLight + ndot * vec3(.9, .9, .9);

	outputColor = vec4(color.xyz * totalLight, color.z);
}