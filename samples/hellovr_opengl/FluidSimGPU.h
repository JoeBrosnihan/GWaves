#pragma once

#include "GLTexture.h"
#include "GLRenderTarget.h"
#include "GLModel.h"
#include "IMaterial.h"
#include "IRenderer.h"

class FluidSimGPU {
public:
	FluidSimGPU(int size, IProgram* displayProgram);
	void step(IRenderer* renderer, float dt);

	GLTexture ut[2], vt[2], pt[2];
	GLRenderTarget u[2], v[2], p[2];

	GLModel internalQuad;

	IMaterial displayMaterial;
};
