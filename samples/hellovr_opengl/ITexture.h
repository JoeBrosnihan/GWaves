#pragma once

enum TextureFormat {RGBA, R32F};

class ITexture {
public:
	ITexture() {};
	ITexture(int width, int height) : width(width), height(height) {};
	int getWidth() { return width; }
	int getHeight() { return height; }
private:
	int width, height;
};