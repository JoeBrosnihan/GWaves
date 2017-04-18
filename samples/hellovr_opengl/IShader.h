#pragma once

#include <string>
#include "ITexture.h"

enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER};

class IShader {
public:
	IShader(ShaderType shaderType) : shaderType(shaderType) {}
	virtual void loadSource(const std::string &source) = 0;
	ShaderType getShaderType() { return shaderType; }
private:
	ShaderType shaderType;
};

class IProgram {
public:
	virtual void link() = 0;
	virtual void useProgram() const = 0;
	virtual void setTexture(const std::string &param, const ITexture* texture) const = 0;
protected:
	const IShader* vertexShader;
	const IShader* fragmentShader;
};
