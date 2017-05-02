#version 410 core

uniform sampler2D texture;

in vec2 texCoord;

out vec4 outputColor;


void main()
{
	outputColor = texture2D(texture, texCoord);
	//outputColor = vec4(0, 0, 1, 1);
}