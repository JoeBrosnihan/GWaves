#pragma once

#include "IModel.h"
#include <GL/glew.h>

class GLModel : public IModel {
public:
	virtual void render();
	virtual void loadBuffers();
private:
	GLuint vertexArrayObject;
	GLuint vertexBuffer;
};