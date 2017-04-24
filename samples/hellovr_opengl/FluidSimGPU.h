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
	void set_bnd(int b, IRenderTarget* x, ITexture* x0);
	void advect(int b, IRenderTarget* x, ITexture* x0, ITexture* u, ITexture* v, float dt);
	void diffuse(int b, IRenderTarget* p, ITexture* p0, float diff, float dt);
	void project(IRenderTarget* u_out, IRenderTarget* v_out, ITexture* u_in, ITexture* v_in, IRenderTarget* aux);

	int TEX_SIZE;
	GLTexture ut[2], vt[2], pt[2];
	GLRenderTarget u[2], v[2], p[2];

	GLShader internalVertShader;

	GLShader diffuseFragShader;
	GLProgram diffuseProgram;
	IMaterial diffuseMaterial;

	GLShader advectFragShader;
	GLProgram advectProgram;
	IMaterial advectMaterial;

	GLShader project1FragShader;
	GLProgram project1Program;
	IMaterial project1Material;

	GLShader project2FragShader;
	GLProgram project2Program;
	IMaterial project2Material;

	GLShader project3FragShader;
	GLProgram project3Program;
	IMaterial project3Material;
};
