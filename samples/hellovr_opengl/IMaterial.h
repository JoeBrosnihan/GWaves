#pragma once

/*
A Material is a shader program together with a set of values for the shader parameters.
*/

#include <vector>
#include <string>
#include <unordered_map>
#include "ITexture.h"
#include "IShader.h"

class IMaterial {
public:
	IMaterial(const IProgram* program) : program(program), textureParameters() {}

	void useMaterial() {
		program->useProgram();
		for (auto it = textureParameters.begin(); it != textureParameters.end(); it++) {
			program->setTexture(it->first, it->second);
		}
	}

	void setTexture(const std::string &param, const ITexture* value) {
		textureParameters[param] = value;
	}
private:
	const IProgram* program;
	std::unordered_map<std::string, const ITexture*> textureParameters;
};
