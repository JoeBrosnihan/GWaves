#pragma once

class ITexture {
public:
	ITexture(int width, int height) : width(width), height(height) {};
protected:
	int width, height;
};