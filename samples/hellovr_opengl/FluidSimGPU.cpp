
#include "FluidSimGPU.h"
#include "IOUtils.h"

FluidSimGPU::FluidSimGPU(int size, IProgram* displayProgram) : internalVertShader(VERTEX_SHADER), diffuseFragShader(FRAGMENT_SHADER)
{
	TEX_SIZE = size;
	
	float* texData = new float[TEX_SIZE * TEX_SIZE];
	for (int i = 0; i < TEX_SIZE * TEX_SIZE; i++) {
		texData[i] = i / (float)(TEX_SIZE * TEX_SIZE);
		texData[i] = (i % 256) / 256.f;
	}

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
		IModel::Vertex(-1, 1, 0, 0, 0, 0, 0, 1),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0));
	internalQuad.addFace(
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(1, -1, 0, 0, 0, 0, 1, 0));
	internalQuad.loadBuffers();

	displayMaterial = IMaterial(displayProgram);
	// this is a perfect example of why it is bad style to put GL initialization in a class constructor.
	displayMaterial.setTexture("texture", (const ITexture*) &pt[1]);

	internalVertShader.loadSource(readFile("fluid/diffuse.vs"));
	diffuseFragShader.loadSource(readFile("fluid/diffuse.fs"));
	diffuseProgram = GLProgram(&internalVertShader, &diffuseFragShader);
	diffuseProgram.link();
	diffuseMaterial = IMaterial(&diffuseProgram);
	diffuseMaterial.setFloat("pixelSize", 1.0f / TEX_SIZE);

	delete[] texData;
}

void FluidSimGPU::set_bnd(int b, GLRenderTarget* x, GLTexture* x0)
{
	
}

void FluidSimGPU::diffuse(IRenderer* renderer, int b, GLRenderTarget* p, GLTexture* p0, float diff, float dt)
{
	internalQuad.setMaterial(&diffuseMaterial);
	float a = dt * diff * TEX_SIZE * TEX_SIZE;
	diffuseMaterial.setFloat("a", a);

	diffuseMaterial.setTexture("texture", p0);
	p->useTarget();
	internalQuad.render();
}

template<typename T>
void swap(T* p)
{
	T temp = p[0];
	p[0] = p[1];
	p[1] = temp;
}

void FluidSimGPU::step(IRenderer* renderer, float dt)
{
	swap(p); swap(pt);
	diffuse(renderer, 0, &p[0], &pt[1], .001f, dt);
	//advect
	
	displayMaterial.setTexture("texture", &pt[0]);
}
