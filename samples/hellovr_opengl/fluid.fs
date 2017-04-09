#version 410 core

in vec2 texCoord;

uniform sampler2D texture;

out vec4 outputColor;

void main()
{
   outputColor = texture2D(texture, texCoord);
}