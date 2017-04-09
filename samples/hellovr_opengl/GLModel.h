#pragma once

#include "IModel.h"
#include <GL/glew.h>

class GLModel : public IModel {
public:
	void render();
	void loadBuffers();
private:
	GLuint vertexArrayObject;
	GLuint vertexBuffer;
};