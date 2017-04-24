#version 410 core

in vec2 texCoord;

uniform sampler2D g_field;
uniform vec2 center;
uniform float jump;

out float outputValue;


void main()
{
	vec2 diff = center - texCoord;
	float dist = length(diff);

	if (dist < jump) {
		dist = jump;
	}
	vec2 dir = diff / dist;

	float force = texture2D(g_field, texCoord + jump * dir).r;
	outputValue = force * (1 - .5 * jump / dist);
}