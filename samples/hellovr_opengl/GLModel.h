#pragma once

#include "IModel.h"
#include <GL/glew.h>

class GLModel : public IModel {
public:
	virtual void render(const Matrix4 &viewProj);
	virtual void loadBuffers();
	GLuint vertexArrayObject;
private:
	GLuint vertexBuffer;
};