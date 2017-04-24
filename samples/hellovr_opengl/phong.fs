#version 410 core

//uniform vec4 color;

in vec2 texCoord;
in vec3 worldPosition;
in vec3 worldNormal;

out vec4 outputColor;


void main()
{
	vec4 color = vec4(.1, .1, .1, .1); // should be a uniform
	//vec4 color = vec4(1, 1, 1, 1); // should be a uniform
	vec3 totalLight = vec3(.05, .05, .05);

	float ndot = dot(worldNormal, vec3(-0.57735, -0.57735, 0.57735));
	ndot = max(ndot, 0.0);
	totalLight = totalLight + ndot * vec3(1, 1, 1);

	outputColor = vec4(color.xyz * totalLight, color.z);
}