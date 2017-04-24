#pragma once

#include "IRenderer.h"
#include "GLModel.h"
#include "GLRenderTarget.h"
#include "GLShader.h"

class GWaveSim {
public:
	GWaveSim(int size, GLProgram* displayProgram);
	void step(IRenderer* renderer, float dt);

	GLModel internalQuad;
	IMaterial displayMaterial;
private:
	int TEX_SIZE;
	GLTexture fieldT[2];
	GLRenderTarget field[2];

	GLShader fieldVertShader;
	GLShader fieldFragShader;
	GLProgram fieldProgram;
	IMaterial fieldMaterial;

	GLModel body;
	GLShader bodyFragShader;
	GLProgram bodyProgram;
	IMaterial bodyMaterial;
};
