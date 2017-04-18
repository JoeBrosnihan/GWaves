
#include "GLShader.h"

GLShader::GLShader(ShaderType shaderType) : IShader(shaderType), shaderObject(0) {};

void GLShader::loadSource(const std::string &source) {
	switch (getShaderType()) {
	case VERTEX_SHADER:
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER:
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}
	GLint sourceLen = source.length();
	GLchar* sourceCStr = (GLchar *) source.c_str();
	glShaderSource(shaderObject, 1, &sourceCStr, &sourceLen);
	glCompileShader(shaderObject);
}



GLProgram::GLProgram(const GLShader* vertexShader, const GLShader* fragmentShader) {
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
	programObject = glCreateProgram();
}

void GLProgram::link() {
	glAttachShader(programObject, static_cast<const GLShader*>(vertexShader)->getShaderObject());
	glAttachShader(programObject, static_cast<const GLShader*>(fragmentShader)->getShaderObject());

	glBindAttribLocation(programObject, 0, "postion"); // should not assume this. In fact, it is not needed since it is declared in the GLSL code. remove this c++ code
	glLinkProgram(programObject);
	glValidateProgram(programObject); //what does this do???
}

void GLProgram::useProgram() const {
	glUseProgram(programObject);
}

void GLProgram::setTexture(const std::string &param, const ITexture* texture) const {
	const GLTexture* glTexture = static_cast<const GLTexture*>(texture);
	// TODO: Support using multiple active textures at once.
	GLuint uniformHandle = glGetUniformLocation(programObject, (GLchar*) param.c_str());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, glTexture->getTextureObject());
	glUniform1i(uniformHandle, 1);
}
