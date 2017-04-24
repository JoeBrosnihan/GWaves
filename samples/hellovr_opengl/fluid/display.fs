#version 410 core

in vec2 texCoord;

uniform sampler2D texture;
uniform sampler2D velX;
uniform sampler2D velY;

out vec4 outputColor;

void main()
{
	float val = texture2D(texture, texCoord).r;
	float vx = texture2D(velX, texCoord).r;
	float vy = texture2D(velY, texCoord).r;
   outputColor = vec4(vx, vy, val, 1.0);
}