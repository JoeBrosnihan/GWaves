#pragma once

#include "IRenderTarget.h"
#include "GLTexture.h"

#include <GL/glew.h>

class GLRenderTarget : public IRenderTarget
{
public:
	GLRenderTarget() {};
	GLRenderTarget(int width, int height, int nSamples);
	GLRenderTarget(GLTexture* target);
	virtual void useTarget() const;
private:
	GLuint framebuffer;
	GLuint depthbuffer;
	GLuint colorbuffer;
};
