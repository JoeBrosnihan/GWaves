#version 410 core

in vec2 texCoord;

uniform sampler2D p;
uniform sampler2D div;
uniform float pixelSize;

out float outputValue;

#define s texture2D
#define px pixelSize

void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;
	outputValue = (
		s(div, texCoord).r +
		s(p, vec2(tx + px, ty)).r +
		s(p, vec2(tx - px, ty)).r +
		s(p, vec2(tx, ty + px)).r +
		s(p, vec2(tx, ty - px)).r
	) / 4.0;
}