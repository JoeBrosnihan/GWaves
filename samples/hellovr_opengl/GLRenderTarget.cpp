
#include "GLRenderTarget.h"

GLRenderTarget::GLRenderTarget(int width, int height, int nSamples)
	: IRenderTarget(width, height, nSamples),
	framebuffer(0),
	depthbuffer(0),
	colorbuffer(0)
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, nSamples, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	glGenTextures(1, &colorbuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorbuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, nSamples, GL_RGBA8, width, height, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorbuffer, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
