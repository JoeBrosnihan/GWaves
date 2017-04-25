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
	GLProgram() {}
	GLProgram(const GLShader* vertexShader, const GLShader* fragmentShader);
	virtual void link();
	virtual void useProgram() const;
	virtual void setTexture(const std::string &param, const ITexture* texture, int slot = 1) const;
	virtual void setFloat(const std::string &param, float value) const;
	virtual void setFloat2(const std::string &param, float v1, float v2) const;
	virtual void setFloat4(const std::string &param, float v1, float v2, float v3, float v4) const;
	virtual void setMat4(const std::string &param, const Matrix4 &value) const;
private:
	GLuint programObject;
};
