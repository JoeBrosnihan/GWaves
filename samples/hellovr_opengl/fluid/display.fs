#version 410 core

in vec2 texCoord;

uniform sampler2D texture;
uniform vec2 pixelMarchVector;

out vec4 outputColor;


#define s texture2D
#define dp pixelMarchVector

void main()
{
	vec4 accum = vec4(0);
	
	accum += 0.054488684549643 * s(texture, texCoord - dp * 2);
	accum += 0.24420134200323 * s(texture, texCoord - dp);
	accum += 0.40261994689425 * s(texture, texCoord);
	accum += 0.24420134200323 * s(texture, texCoord + dp);
	accum += 0.054488684549643 * s(texture, texCoord + dp * 2);
	
	// We perform the 2nd pass of the gaussian blur above in this display shader to save writing to an extra framebuffer.
	
	float val = accum.r;
	outputColor = vec4(val, .5 * val * val * val, 0.0, 1.0);
}