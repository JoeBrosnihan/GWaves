#pragma once

#include "ITexture.h"

#include <GL/glew.h>

class GLTexture : public ITexture {
public:
	GLTexture(int width, int height, const char* texData);
	void update(const char* texData);
	GLuint getTextureObject() const { return texture; };
private:
	GLuint texture;
};
