
#include "FluidSimGPU.h"
#include "IOUtils.h"

FluidSimGPU::FluidSimGPU(int size, IProgram* displayProgram) : internalVertShader(VERTEX_SHADER), diffuseFragShader(FRAGMENT_SHADER), advectFragShader(FRAGMENT_SHADER)
	, project1FragShader(FRAGMENT_SHADER), project2FragShader(FRAGMENT_SHADER), project3FragShader(FRAGMENT_SHADER)
{
	TEX_SIZE = size;

	float* dens = new float[TEX_SIZE * TEX_SIZE];
	float* vel = new float[TEX_SIZE * TEX_SIZE];
	for (int i = 0; i < TEX_SIZE * TEX_SIZE; i++) {
		dens[i] = 0.1f;
		vel[i] = 0.5f;

		int j = i % TEX_SIZE;
		int k = i / TEX_SIZE;
		if (j > 150 && j < 200 && k > 150 && k < 200)
			dens[i] = 1;
		if (j > 50 && j < 300 && k > 50 && k < 300)
			vel[i] = 1;
	}

	for (int i = 0; i <= 1; i++)
	{
		ut[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) vel);
		u[i] = GLRenderTarget(&ut[i]);
		vt[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) vel);
		v[i] = GLRenderTarget(&vt[i]);
		pt[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) dens);
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
	//displayMaterial.setTexture("texture", (const ITexture*) &pt[1]);
	displayMaterial.setTexture("texture", p[0].getTexture());
	displayMaterial.setTexture("velX", u[0].getTexture());
	displayMaterial.setTexture("velY", v[0].getTexture());

	internalVertShader.loadSource(readFile("fluid/diffuse.vs"));

	diffuseFragShader.loadSource(readFile("fluid/diffuse.fs"));
	diffuseProgram = GLProgram(&internalVertShader, &diffuseFragShader);
	diffuseProgram.link();
	diffuseMaterial = IMaterial(&diffuseProgram);
	diffuseMaterial.setFloat("pixelSize", 1.0f / TEX_SIZE);

	advectFragShader.loadSource(readFile("fluid/advect.fs"));
	advectProgram = GLProgram(&internalVertShader, &advectFragShader);
	advectProgram.link();
	advectMaterial = IMaterial(&advectProgram);

	project1FragShader.loadSource(readFile("fluid/project1.fs"));
	project1Program = GLProgram(&internalVertShader, &project1FragShader);
	project1Program.link();
	project1Material = IMaterial(&project1Program);
	project1Material.setFloat("pixelSize", 1.0f / TEX_SIZE);

	project2FragShader.loadSource(readFile("fluid/project2.fs"));
	project2Program = GLProgram(&internalVertShader, &project2FragShader);
	project2Program.link();
	project2Material = IMaterial(&project2Program);
	project2Material.setFloat("pixelSize", 1.0f / TEX_SIZE);

	project3FragShader.loadSource(readFile("fluid/project3.fs"));
	project3Program = GLProgram(&internalVertShader, &project3FragShader);
	project3Program.link();
	project3Material = IMaterial(&project3Program);
	project3Material.setFloat("pixelSize", 1.0f / TEX_SIZE);

	delete[] dens;
	delete[] vel;
}

void FluidSimGPU::set_bnd(int b, IRenderTarget* x, ITexture* x0)
{
	
}

void FluidSimGPU::advect(int b, IRenderTarget* x, ITexture* x0, ITexture* u, ITexture* v, float dt) {
	internalQuad.setMaterial(&advectMaterial);
	advectMaterial.setFloat("dt", dt);
	advectMaterial.setTexture("texture", x0);
	advectMaterial.setTexture("velX", u); // am I mixing up u and v?
	advectMaterial.setTexture("velY", v);
	x->useTarget();
	internalQuad.render();
}

void FluidSimGPU::diffuse(int b, IRenderTarget* p, ITexture* p0, float diff, float dt)
{
	internalQuad.setMaterial(&diffuseMaterial);
	float a = dt * diff * TEX_SIZE * TEX_SIZE;
	diffuseMaterial.setFloat("a", a);

	diffuseMaterial.setTexture("texture", p0);
	p->useTarget();
	internalQuad.render();

	set_bnd(b, p, p0);
}

void FluidSimGPU::project(IRenderTarget* u_out, IRenderTarget* v_out, ITexture* u_in, ITexture* v_in, IRenderTarget* aux) {
	// u_in, v_in, aux0=u_out, aux1, aux2=v_out
	internalQuad.setMaterial(&project1Material);
	// read u_in, v_in to write to u_out.
	project1Material.setFloat("pixelSize", 1.0f / TEX_SIZE);
	project1Material.setTexture("velX", u_in);
	project1Material.setTexture("velY", v_in);
	u_out->useTarget();
	internalQuad.render();

	// write aux1 = 0
	project1Material.setFloat("pixelSize", 0.0f);
	aux->useTarget();
	internalQuad.render();

	internalQuad.setMaterial(&project2Material);
	project2Material.setTexture("div", u_out->getTexture());
	for (int k = 0; k < 10; k++) {
		// read u_out, aux(i) to write to aux(j).
		// where i,j alternate between 1,2 and 2,1.
		project2Material.setTexture("p", aux->getTexture());
		v_out->useTarget();
		internalQuad.render();

		project2Material.setTexture("p", v_out->getTexture());
		aux->useTarget();
		internalQuad.render();
	}
	// p = aux1 at the end of this loop

	// read u_in, v_in, aux1 to write to u_out, v_out
	internalQuad.setMaterial(&project3Material);
	project3Material.setTexture("p", aux->getTexture());

	project3Material.setTexture("vel", u_in);
	project3Material.setFloat("xWeight", 1);
	u_out->useTarget();
	internalQuad.render();

	project3Material.setTexture("vel", v_in);
	project3Material.setFloat("xWeight", 0);
	v_out->useTarget();
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
	const float diffusion = .001f;
	const float viscosity = .001f;
	//Step Dens
	swap(p);
	diffuse(0, &p[0], p[1].getTexture(), .001f, dt);
	swap(p);
	advect(0, &p[0], p[1].getTexture(), u[0].getTexture(), v[0].getTexture(), dt);

	//Step Vel
	swap(u);
	diffuse(1, &u[0], u[1].getTexture(), viscosity, dt);
	swap(v);
	diffuse(2, &v[0], v[1].getTexture(), viscosity, dt);
	swap(u);
	swap(v);
	project(&u[0], &v[0], u[1].getTexture(), v[1].getTexture(), &p[1]);

	swap(u);
	swap(v);
	advect(1, &u[0], u[1].getTexture(), u[1].getTexture(), v[1].getTexture(), dt);
	advect(2, &v[0], v[1].getTexture(), u[1].getTexture(), v[1].getTexture(), dt);
	swap(u);
	swap(v);
	project(&u[0], &v[0], u[1].getTexture(), v[1].getTexture(), &p[1]);
	

	displayMaterial.setTexture("texture", p[0].getTexture());
	displayMaterial.setTexture("velX", u[0].getTexture());
	displayMaterial.setTexture("velY", v[0].getTexture());
}
