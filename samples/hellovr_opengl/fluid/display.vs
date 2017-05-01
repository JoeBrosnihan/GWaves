#version 410

uniform sampler2D texture;
uniform mat4 mat_model;
uniform mat4 mat_viewproj;
uniform vec2 pixelMarchVector;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec2 texCoord;

#define s texture2D
#define dp pixelMarchVector

void main()
{
	texCoord = uvCoord;
	
	vec4 accum = vec4(0);
	
	accum += 0.054488684549643 * s(texture, texCoord - dp * 2);
	accum += 0.24420134200323 * s(texture, texCoord - dp);
	accum += 0.40261994689425 * s(texture, texCoord);
	accum += 0.24420134200323 * s(texture, texCoord + dp);
	accum += 0.054488684549643 * s(texture, texCoord + dp * 2);
	
	// We perform the 2nd pass of the gaussian blur above in this display shader to save writing to an extra framebuffer.
	
	float val = accum.r;
	
	//float val = texture2D(texture, uvCoord).r;
	gl_Position = mat_viewproj * (mat_model * vec4(position + vec3(0.0, val * .1, 0.0), 1.0));
}