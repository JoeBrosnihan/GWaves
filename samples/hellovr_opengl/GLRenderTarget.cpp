
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

GLRenderTarget::GLRenderTarget(GLTexture* target) : IRenderTarget(target) {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Do I need to create a depth buffer every time? If so, do that here.

	glBindTexture(GL_TEXTURE_2D, target->getTextureObject());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target->getTextureObject(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderTarget::useTarget() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}
