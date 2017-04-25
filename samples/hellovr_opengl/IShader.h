#pragma once

#include <string>
#include "ITexture.h"
#include "Shared/Matrices.h"

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
	IProgram() : vertexShader(nullptr), fragmentShader(nullptr) {}
	virtual void link() = 0;
	virtual void useProgram() const = 0;
	virtual void setTexture(const std::string &param, const ITexture* texture, int slot = 1) const = 0;
	virtual void setFloat(const std::string &param, float value) const = 0;
	virtual void setFloat2(const std::string &param, float v1, float v2) const = 0;
	virtual void setFloat4(const std::string &param, float v1, float v2, float v3, float v4) const = 0;
	virtual void setMat4(const std::string &param, const Matrix4 &value) const = 0;
protected:
	const IShader* vertexShader;
	const IShader* fragmentShader;
};
