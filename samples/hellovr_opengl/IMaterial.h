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
	IMaterial() : program(nullptr), textureParameters(), floatParameters() {}

	IMaterial(const IProgram* program) : program(program), textureParameters(), floatParameters() {}

	void useMaterial() {
		program->useProgram();
		for (size_t i = 0; i < textureParameters.size(); i++) {
			tex_keyval entry = textureParameters[i];
			program->setTexture(entry.first, entry.second, i + 1);
		}
		for (auto it = floatParameters.begin(); it != floatParameters.end(); it++) {
			program->setFloat(it->first, it->second);
		}
	}

	void setTexture(const std::string &param, const ITexture* value) {
		for (auto it = textureParameters.begin(); it != textureParameters.end(); it++) {
			if (it->first == param) {
				it->second = value;
				return;
			}
		}
		textureParameters.push_back(tex_keyval(param, value));
	}

	void setFloat(const std::string &param, float value)
	{
		floatParameters[param] = value;
	}
private:
	typedef std::pair<std::string, const ITexture*> tex_keyval;
	const IProgram* program;
	std::vector<tex_keyval> textureParameters;
	std::unordered_map<std::string, float> floatParameters;
};
