#version 410 core

in vec2 texCoord;

uniform sampler2D texture;

out vec4 outputColor;

void main()
{
	float val = texture2D(texture, texCoord).r;
	outputColor = vec4(val, 0.0, 0.0, 1.0);
}