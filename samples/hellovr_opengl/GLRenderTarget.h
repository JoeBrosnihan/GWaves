#pragma once

#include <GL/glew.h>

#include "IRenderTarget.h"

class GLRenderTarget : IRenderTarget
{
public:
	GLRenderTarget(int width, int height, int nSamples);
private:
	GLuint framebuffer;
	GLuint depthbuffer;
	GLuint colorbuffer;
};
