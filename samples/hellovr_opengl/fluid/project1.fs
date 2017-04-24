#version 410 core

in vec2 texCoord;

uniform sampler2D velX;
uniform sampler2D velY;
uniform float pixelSize;

out float outputValue;

#define s texture2D
#define px pixelSize

void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;
	outputValue = -5.0 * px * (
		s(velX, vec2(tx + px, ty)).r -
		s(velX, vec2(tx - px, ty)).r +
		s(velY, vec2(tx, ty + px)).r -
		s(velY, vec2(tx, ty - px)).r
	);
}