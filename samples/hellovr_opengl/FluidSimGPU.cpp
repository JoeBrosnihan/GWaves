
#include "FluidSimGPU.h"

FluidSimGPU::FluidSimGPU(int size, IProgram* displayProgram)
{
	int TEX_SIZE = size;
	
	float* texData = new float[TEX_SIZE * TEX_SIZE];
	for (int i = 0; i < TEX_SIZE * TEX_SIZE; i++) {
		texData[i] = i / (float)(TEX_SIZE * TEX_SIZE);
	}
	/*
	char* texData = new char[TEX_SIZE * TEX_SIZE * 4];
	for (int i = 0; i < TEX_SIZE * TEX_SIZE * 4; i++) {
		texData[i] = i % 256;
	}
	*/

	for (int i = 0; i <= 1; i++)
	{
		ut[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) texData);
		u[i] = GLRenderTarget(&ut[i]);
		vt[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) texData);
		v[i] = GLRenderTarget(&vt[i]);
		pt[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) texData);
		p[i] = GLRenderTarget(&pt[i]);
	}

	internalQuad = GLModel();
	internalQuad.addFace(
		IModel::Vertex(-.5, .5, 0, 0, 0, 0, 0, 0),
		IModel::Vertex(.5, .5, 0, 0, 0, 0, 1, 0),
		IModel::Vertex(-.5, -.5, 0, 0, 0, 0, 0, 1));
	internalQuad.addFace(
		IModel::Vertex(-.5, -.5, 0, 0, 0, 0, 0, 1),
		IModel::Vertex(.5, .5, 0, 0, 0, 0, 1, 0),
		IModel::Vertex(.5, -.5, 0, 0, 0, 0, 1, 1));
	internalQuad.loadBuffers();

	displayMaterial = IMaterial(displayProgram);
	// this is a perfect example of why it is bad style to put GL initialization in a class constructor.
	displayMaterial.setTexture("texture", (const ITexture*) &pt[0]);

	delete[] texData;
}

void FluidSimGPU::step(IRenderer* renderer, float dt)
{
	
}
