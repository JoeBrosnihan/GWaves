#version 410 core

in vec2 texCoord;

uniform float force;

out float outputValue;


void main()
{
	vec2 diff = vec2(0.5, 0.5) - texCoord;
	float dist = length(diff);

	if (dist > 0.5) {
		discard;
	}

	outputValue = force;
}