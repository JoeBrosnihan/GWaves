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

	const IProgram* getProgram() { return program; }

	void useMaterial() {
		program->useProgram();
		for (size_t i = 0; i < textureParameters.size(); i++) {
			tex_keyval entry = textureParameters[i];
			program->setTexture(entry.first, entry.second, i + 1);
		}
		for (auto it = floatParameters.begin(); it != floatParameters.end(); it++) {
			program->setFloat(it->first, it->second);
		}
		for (auto it = float2Parameters.begin(); it != float2Parameters.end(); it++) {
			program->setFloat2(it->first, it->second.x, it->second.y);
		}
		for (auto it = float4Parameters.begin(); it != float4Parameters.end(); it++) {
			program->setFloat4(it->first, it->second.x, it->second.y, it->second.z, it->second.w);
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

	void setFloat2(const std::string &param, float v1, float v2)
	{
		float2 value = { v1, v2 };
		float2Parameters[param] = value;
	}

	void setFloat4(const std::string &param, float v1, float v2, float v3, float v4)
	{
		float4 value = { v1, v2, v3, v4 };
		float4Parameters[param] = value;
	}
private:
	struct float2 {
		float x, y;
	};
	struct float4 {
		float x, y, z, w;
	};
	typedef std::pair<std::string, const ITexture*> tex_keyval;

	const IProgram* program;
	std::vector<tex_keyval> textureParameters;
	std::unordered_map<std::string, float> floatParameters;
	std::unordered_map<std::string, float2> float2Parameters;
	std::unordered_map<std::string, float4> float4Parameters;
};
