
#include "GLShader.h"
#include <iostream>
#include "IOUtils.h"

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

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		dprintf("Unable to compile shader!\n");

		GLchar infoLog[5000] = { 0 };
		GLint infoLogLength;
		glGetShaderInfoLog(shaderObject, sizeof(infoLog) - 1, &infoLogLength, infoLog);
		dprintf("%s\n", infoLog);

		glDeleteShader(shaderObject);
	}
}



GLProgram::GLProgram(const GLShader* vertexShader, const GLShader* fragmentShader) {
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
	programObject = glCreateProgram();
}

void GLProgram::link() {
	glAttachShader(programObject, static_cast<const GLShader*>(vertexShader)->getShaderObject());
	glAttachShader(programObject, static_cast<const GLShader*>(fragmentShader)->getShaderObject());

	glLinkProgram(programObject);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(programObject, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		dprintf("Error linking program!\n");

		GLchar infoLog[5000] = { 0 };
		GLint infoLogLength;
		glGetProgramInfoLog(programObject, sizeof(infoLog) - 1, &infoLogLength, infoLog);
		dprintf("%s\n", infoLog);

		glDeleteProgram(programObject);
	}

	glValidateProgram(programObject); //what does this do???
	glGetProgramiv(programObject, GL_VALIDATE_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		dprintf("Error validating program!\n");
		//glDeleteProgram(programObject);
	}
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

void GLProgram::setFloat(const std::string &param, float value) const
{
	GLuint uniformHandle = glGetUniformLocation(programObject, (GLchar*)param.c_str());
	glUniform1f(uniformHandle, value);
}
