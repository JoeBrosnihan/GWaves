#version 410
uniform mat4 matrix;
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 v2UVcoordsIn;
layout(location = 2) in vec3 v3NormalIn;
out vec2 v2UVcoords;
void main()
{
	v2UVcoords = v2UVcoordsIn;
	//vec4 coolPosition = position;
	vec4 coolPosition = vec4(position.x + sin(position.y * .5), position.y + sin(position.z * .5), position.z + sin(position.x * .5), position.w);
	gl_Position = matrix * coolPosition;
}