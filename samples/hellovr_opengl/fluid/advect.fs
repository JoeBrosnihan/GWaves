#version 410 core

in vec2 texCoord;

uniform sampler2D texture;
uniform sampler2D velX;
uniform sampler2D velY;
uniform float dt;

out float outputValue;

//http://seanmiddleditch.com/

void main()
{
	float tx = texCoord.x;
	float ty = texCoord.y;

	float sx = tx - dt * texture2D(velX, texCoord).r;
	float sy = ty - dt * texture2D(velY, texCoord).r;

	//boundary check here

	outputValue = texture2D(texture, vec2(sx, sy)).r;
}