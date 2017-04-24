//========= Copyright Valve Corporation ============//

#include <SDL.h>

#include <iostream>

#include "SDLDisplay.h"
#include "GLRenderer.h"
#include "GLRenderTarget.h"
#include "IOUtils.h"
#include "IModel.h"
#include "GWaveSim.h"

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
	
	GLModel quad;
	quad.addFace(
		IModel::Vertex(-1, 1, 0, 0, 0, 0, 0, 1),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0));
	quad.addFace(
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(1, -1, 0, 0, 0, 0, 1, 0));
	quad.loadBuffers();
	quad.transform.scale(.75f);
	
	// Material

	GLShader vertShader(VERTEX_SHADER);
	vertShader.loadSource(readFile("fluid.vs"));

	GLShader fragShader(FRAGMENT_SHADER);
	fragShader.loadSource(readFile("fluid/display.fs"));

	GLProgram fluidProgram(&vertShader, &fragShader);
	fluidProgram.link();

	renderer.addModel(&quad);

	//FluidSimGPU fluidSim(1024, &fluidProgram);
	//quad.setMaterial(&fluidSim.displayMaterial);

	GWaveSim gWaveSim(1024, &fluidProgram);
	quad.setMaterial(&gWaveSim.displayMaterial);

	while (!display.isClosed()) {
		renderer.renderToDisplay();

		renderer.updateDisplay();
		gWaveSim.step(&renderer, .016f);
	}

	return 0;
}
