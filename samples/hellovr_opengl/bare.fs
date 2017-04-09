#version 410 core
uniform sampler2D mytexture;
in vec2 v2UVcoords;
out vec4 outputColor;
void main()
{
   outputColor = vec4(1.0, 0.5, 0.5, 1.0);
}