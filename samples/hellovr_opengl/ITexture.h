#pragma once

class ITexture {
public:
	ITexture(int width, int height) : width(width), height(height) {};
	int getWidth() { return width; }
	int getHeight() { return height; }
private:
	int width, height;
};