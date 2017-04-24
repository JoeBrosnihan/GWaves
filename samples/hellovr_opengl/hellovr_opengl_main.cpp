//========= Copyright Valve Corporation ============//

#include <SDL.h>

#include <iostream>

#include "SDLDisplay.h"
#include "GLRenderer.h"
#include "GLRenderTarget.h"
#include "IOUtils.h"
#include "FluidSim.h"
#include "IModel.h"
#include "FluidSimGPU.h"

#include "GLTexture.h"

// Use the OpenGL implementation.
typedef SDLDisplay Display;
typedef GLRenderer Renderer;
typedef GLTexture Texture;

bool handleInput()
{
	SDL_Event sdlEvent;
	bool bRet = false;

	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		if (sdlEvent.type == SDL_QUIT)
		{
			bRet = true;
		}
		else if (sdlEvent.type == SDL_KEYDOWN)
		{
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE
				|| sdlEvent.key.keysym.sym == SDLK_q)
			{
				bRet = true;
			}
		}
	}
	return bRet;
}

void updateFluid(FluidSim &fluid, Texture& texture, char * texData, int texSize) {
	float avgDensity = 0;
	for (int i = 0; i < texSize; i++) {
		for (int j = 0; j < texSize; j++) {
			int pixelIndex = i * texSize + j;
			int fluidIndex = (i + 1) * (texSize + 2) + j + 1;
			avgDensity += fluid.p[fluidIndex];
			uint8_t val = (char)(fluid.p[fluidIndex] * 256);
			texData[pixelIndex * 4] = (char)(fluid.p[fluidIndex] * 256);
			texData[pixelIndex * 4 + 1] = 0;
			texData[pixelIndex * 4 + 2] = 0;
			texData[pixelIndex * 4 + 3] = 0;
		}
	}
	avgDensity /= texSize * texSize;
	std::cout << "avg p: " << avgDensity << "\n";

	texture.update(texData);

	float dt = .02f; //figure this out later
	fluid.stepVel(dt, 0);
	fluid.stepDens(dt, 0);
}

int main(int argc, char *argv[])
{
	/*
	Interestingly, the following does not work. I think it tries to use a copy
	constructor because the compiler thinks the types are technically not equal.
	Renderer<GLRenderer> renderer = GLRenderer();
	This is the fix:
	GLRenderer renderer = GLRenderer();
	*/
	Display display(800, 600, "hello sdl");
	Renderer renderer(&display);
	renderer.init();
	
	/*GLRenderTarget target(300, 200, 4);
	GLModel cubes;
	cubes.cubeTest();
	cubes.loadBuffers();*/

	//BEGIN FLUID SIM CODEBLOCK
	
	GLModel quad;
	quad.addFace(
		IModel::Vertex(-.5, .5, 0, 0, 0, 0, 0, 1),
		IModel::Vertex(.5, .5, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(-.5, -.5, 0, 0, 0, 0, 0, 0));
	quad.addFace(
		IModel::Vertex(-.5, -.5, 0, 0, 0, 0, 0, 0),
		IModel::Vertex(.5, .5, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(.5, -.5, 0, 0, 0, 0, 1, 0));
	quad.loadBuffers();
	
	// Material

	GLShader vertShader(VERTEX_SHADER);
	vertShader.loadSource(readFile("fluid.vs"));

	GLShader fragShader(FRAGMENT_SHADER);
	fragShader.loadSource(readFile("fluid/display.fs"));

	GLProgram fluidProgram(&vertShader, &fragShader);
	fluidProgram.link();

	renderer.addModel(&quad);

	FluidSimGPU fluidSim(1024, &fluidProgram);
	quad.setMaterial(&fluidSim.displayMaterial);

	/*
	//Fluid

	FluidSim fluid(TEX_SIZE);
	for (int i = 1; i <= TEX_SIZE; i++) {
		for (int j = 1; j <= TEX_SIZE; j++) {
			int k = i * (TEX_SIZE + 2) + j;
			if (i > 10 && i < 20 && j > 10 && j < 20) {
				fluid.p[k] = .9;
				fluid.u[k] = fluid.v[k] = 10;
			}
			else {
				fluid.p[k] = 0;
				fluid.u[k] = fluid.v[k] = -10;
			}
		}
	}
	*/
	// END FLUID SIM CODEBLOCK

	while (!display.isClosed()) {
		renderer.renderToDisplay();

		renderer.updateDisplay();
		fluidSim.step(&renderer, .016f);
	}

	return 0;
}
