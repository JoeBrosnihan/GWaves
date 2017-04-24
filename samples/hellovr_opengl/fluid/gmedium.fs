#version 410 core

in vec2 texCoord;

uniform sampler2D g_field;
uniform vec2 center;
uniform float jump;

out float outputValue;


void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;

	vec2 diff = center - texCoord;
	float dist = length(diff);

	if (dist < jump) {
		dist = jump;
	}
	vec2 dir = diff / dist;

	outputValue = texture2D(g_field, texCoord + jump * dir).r;
}