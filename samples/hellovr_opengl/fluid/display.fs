#version 410 core

in vec2 texCoord;

uniform sampler2D texture;

out vec4 outputColor;

//http://seanmiddleditch.com/

void main()
{
	float val = texture2D(texture, texCoord).r;
   outputColor = vec4(0.0, 0.0, val, 1.0);
   //outputColor = texture2D(texture, texCoord);
}