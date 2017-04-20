#pragma once

#include "GLTexture.h"
#include "GLRenderTarget.h"
#include "GLModel.h"
#include "IMaterial.h"
#include "IRenderer.h"
#include "GLShader.h"

class FluidSimGPU {
public:
	FluidSimGPU(int size, IProgram* displayProgram);
	void step(IRenderer* renderer, float dt);

	GLModel internalQuad;

	IMaterial displayMaterial;
private:
	void set_bnd(int b, GLRenderTarget* x, GLTexture* x0);
	void diffuse(IRenderer* renderer, int b, GLRenderTarget* p, GLTexture* p0, float diff, float dt);

	int TEX_SIZE;
	GLTexture ut[2], vt[2], pt[2];
	GLRenderTarget u[2], v[2], p[2];

	GLShader internalVertShader;

	GLShader diffuseFragShader;
	GLProgram diffuseProgram;
	IMaterial diffuseMaterial;
};
