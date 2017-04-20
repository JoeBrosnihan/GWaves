#version 410 core

in vec2 texCoord;

uniform sampler2D texture;
uniform float pixelSize;
uniform float a;

out float outputValue;

//http://seanmiddleditch.com/

#define t texture
#define s texture2D
#define px pixelSize

void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;
	outputValue = (
		s(t, vec2(tx, ty)).r + a * (
			s(t, vec2(tx - px, ty)).r +
			s(t, vec2(tx + px, ty)).r +
			s(t, vec2(tx, ty - px)).r +
			s(t, vec2(tx, ty + px)).r
		)
	) / (1 + 4 * a);
}