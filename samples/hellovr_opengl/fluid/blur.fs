#version 410 core

in vec2 texCoord;

uniform sampler2D source;
uniform vec2 pixelMarchVector;

out float outputValue;

#define s texture2D
#define px pixelSize
#define dp pixelMarchVector

void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;
	
	vec4 accum = vec4(0);
	
	accum += 0.054488684549643 * s(source, texCoord - dp * 2);
	accum += 0.24420134200323 * s(source, texCoord - dp);
	accum += 0.40261994689425 * s(source, texCoord);
	accum += 0.24420134200323 * s(source, texCoord + dp);
	accum += 0.054488684549643 * s(source, texCoord + dp * 2);
	
	outputValue = accum.r;
}


/*
N=5, sigma=1, x=[-2,2]
0.054488684549643
0.24420134200323
0.40261994689425
0.24420134200323
0.054488684549643

N=5, sigma=1.1, x=[-2,2]
0.070766371331546
0.24446039891162
0.36954645951366
0.24446039891162
0.070766371331546
*/