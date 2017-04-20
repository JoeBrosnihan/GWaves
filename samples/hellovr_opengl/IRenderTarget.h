#pragma once

/*
Holds a render target framebuffer.
*/

#include "ITexture.h"

class IRenderTarget {
public:
	IRenderTarget() {};

	IRenderTarget(int width, int height, int nSamples)
		: target(nullptr),
		width(width),
		height(height),
		nSamples(nSamples) {};

	IRenderTarget(ITexture* target)
		: target(target),
		width(target->getWidth()),
		height(target->getHeight()),
		nSamples(1) {};

	virtual void useTarget() = 0;

	int getWidth() { return width; };
	int getHeight() { return height; };
private:
	int width;
	int height;
	int nSamples;
	ITexture* target;
};
