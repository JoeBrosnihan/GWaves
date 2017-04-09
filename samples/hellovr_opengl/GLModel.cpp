
#include "GLModel.h"
#include <GL/glew.h>

void GLModel::render() {
	glBindVertexArray(vertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//my best guess to extend:
	//glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void GLModel::loadBuffers() {
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), (GLvoid*) vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}
