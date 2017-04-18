#pragma once

#include "IRenderTarget.h"

#include <GL/glew.h>

class GLRenderTarget : IRenderTarget
{
public:
	GLRenderTarget(int width, int height, int nSamples);
private:
	GLuint framebuffer;
	GLuint depthbuffer;
	GLuint colorbuffer;
};
