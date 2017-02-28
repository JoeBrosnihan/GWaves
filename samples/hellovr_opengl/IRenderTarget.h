#pragma once

/*
Holds a render target framebuffer.
*/

class IRenderTarget {
public:
	IRenderTarget(int width, int height, int nSamples)
		: width(width),
		height(height),
		nSamples(nSamples) {};

	int getWidth() { return width; };
	int getHeight() { return height; };
private:
	int width;
	int height;
	int nSamples;
};
