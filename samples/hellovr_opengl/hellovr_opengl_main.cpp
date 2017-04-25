//========= Copyright Valve Corporation ============//

#include <SDL.h>

#include <iostream>

#include "OpenVRDisplay.h"
#include "GLRenderer.h"
#include "GLRenderTarget.h"
#include "IOUtils.h"
#include "IModel.h"
#include "GWaveSim.h"
#include "glm/ext.hpp"

#include "GLTexture.h"


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
	//SDLDisplay display(1800, 1000, "hello sdl");
	OpenVRDisplay display(1800, 1000, "hello sdl");
	GLRenderer renderer(&display);
	renderer.init();
	// HACK: use a glm projection matrix among Valve Matrices because I don't want to write my own projection matrix code.
	glm::mat4 proj = glm::perspective(3.14159f * .5f, display.getWidth() / (float) display.getHeight(), .1f, 100.f);
	renderer.projection = ((Matrix4*) &proj)[0];
	renderer.view.rotateX(-45.0f);
	renderer.view.translate(Vector3(0, 0, -1.5));
	
	// Gravity Field

	GLModel surface;
	const int RESOLUTION = 256;
	const float step = 2.0f / RESOLUTION;
	const float tstep = 1.0f / RESOLUTION;
	for (int i = 0; i < RESOLUTION; i++) {
		for (int j = 0; j < RESOLUTION; j++) {
			float x = i * step - 1;
			float y = j * step - 1;
			float u = i * tstep;
			float v = j * tstep;
			surface.addFace(
				// upper left
				IModel::Vertex(x, y + step, 0, 0, 0, 0, u, v + tstep),
				// upper right
				IModel::Vertex(x + step, y + step, 0, 0, 0, 0, u + tstep, v + tstep),
				// lower left
				IModel::Vertex(x, y, 0, 0, 0, 0, u, v));
			surface.addFace(
				// lower left
				IModel::Vertex(x, y, 0, 0, 0, 0, u, v),
				// upper right
				IModel::Vertex(x + step, y + step, 0, 0, 0, 0, u + tstep, v + tstep),
				// lower right
				IModel::Vertex(x + step, y, 0, 0, 0, 0, u + tstep, v));
		}
	}
	surface.loadBuffers();

	GLShader vertShader(VERTEX_SHADER);
	vertShader.loadSource(readFile("fluid/display.vs"));

	GLShader fragShader(FRAGMENT_SHADER);
	fragShader.loadSource(readFile("fluid/display.fs"));

	GLProgram waveProgram(&vertShader, &fragShader);
	waveProgram.link();

	GWaveSim gWaveSim(1024, &waveProgram);
	surface.setMaterial(&gWaveSim.displayMaterial);

	renderer.addModel(&surface);

	// Black Hole

	GLModel blackHole;
	blackHole.addSphere(.025f);
	blackHole.loadBuffers();

	GLModel blackHole2;
	blackHole2.addSphere(.025f);
	blackHole2.loadBuffers();

	GLShader phongVertShader(VERTEX_SHADER);
	phongVertShader.loadSource(readFile("phong.vs"));
	GLShader phongFragShader(FRAGMENT_SHADER);
	phongFragShader.loadSource(readFile("phong.fs"));
	GLProgram phongProgram(&phongVertShader, &phongFragShader);
	phongProgram.link();

	IMaterial holeMaterial(&phongProgram);
	blackHole.setMaterial(&holeMaterial);
	renderer.addModel(&blackHole);
	blackHole2.setMaterial(&holeMaterial);
	renderer.addModel(&blackHole2);

	//FluidSimGPU fluidSim(1024, &fluidProgram);
	//quad.setMaterial(&fluidSim.displayMaterial);

	long lastTime = SysTimeMS();
	while (!display.isClosed()) {
		long currTime = SysTimeMS();
		float deltaT = (currTime - lastTime) / 1000.f;
		if (deltaT < 0)
			lastTime = lastTime + 0;
		lastTime = currTime;
		gWaveSim.step(&renderer, .0025f); // This needs to be time independent without looking bad!

		float time = currTime / 1000.f;
		const float r = .05f, w = 5.f;
		blackHole.transform = Matrix4();
		blackHole.transform.translate(Vector3(r * cos(w * time), r * sin(w * time), .15f));
		blackHole2.transform = Matrix4();
		blackHole2.transform.translate(Vector3(-r * cos(w * time), -r * sin(w * time), .15f));


		renderer.view = display.GetUpdatedHMDMatrixPose();
		//renderer.projection = display.m_mat4ProjectionLeft;


		renderer.renderToDisplay();
		renderer.updateDisplay();
	}

	return 0;
}
