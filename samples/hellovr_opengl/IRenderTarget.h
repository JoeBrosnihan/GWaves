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

	virtual void useTarget() const = 0;

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	ITexture* getTexture() { return target; }
private:
	int width;
	int height;
	int nSamples;
	ITexture* target;
};
