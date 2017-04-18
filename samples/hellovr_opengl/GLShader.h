#pragma once

#include "IShader.h"

#include <GL/glew.h>
#include <string>
#include "GLRenderer.h"
#include "GLTexture.h"

class GLShader : public IShader {
public:
	GLShader(ShaderType shaderType);
	virtual void loadSource(const std::string &source);
	GLuint getShaderObject() const { return shaderObject; }
private:
	GLuint shaderObject;
};

class GLProgram : public IProgram {
public:
	GLProgram(const GLShader* vertexShader, const GLShader* fragmentShader);
	virtual void link();
	virtual void useProgram() const;
	virtual void setTexture(const std::string &param, const ITexture* texture) const;
private:
	GLuint programObject;
};
