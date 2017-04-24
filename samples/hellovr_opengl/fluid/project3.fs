#version 410 core

in vec2 texCoord;

uniform sampler2D vel;
uniform sampler2D p;
uniform float pixelSize;
uniform float xWeight;

out float outputValue;

#define s texture2D
#define px pixelSize

void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;
	outputValue = s(vel, texCoord).r -
	0.5 * (
		xWeight *
		(s(vel, vec2(tx + px, ty)).r -
		s(vel, vec2(tx - px, ty)).r)
		+ (1.0 - xWeight) *
		(s(vel, vec2(tx, ty + px)).r -
		s(vel, vec2(tx, ty - px)).r)
	) / px;
}