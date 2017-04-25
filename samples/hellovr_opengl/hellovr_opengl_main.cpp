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

void GFieldVisualization() {
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
	glm::mat4 proj = glm::perspective(3.14159f * .5f, display.getWidth() / (float)display.getHeight(), .1f, 100.f);
	renderer.projection = ((Matrix4*)&proj)[0];
	//renderer.view.rotateX(-45.0f);
	renderer.view.translate(Vector3(0, -1, -2));

	// Gravity Field

	GLModel surface;
	const int RESOLUTION = 256;
	surface.addPlane(RESOLUTION, Matrix4().rotateX(-90));
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
		blackHole.transform.translate(Vector3(r * cos(w * time), .15f, -r * sin(w * time)));
		blackHole2.transform = Matrix4();
		blackHole2.transform.translate(Vector3(-r * cos(w * time), .15f, r * sin(w * time)));


		//renderer.view = display.GetUpdatedHMDMatrixPose();
		//renderer.projection = display.m_mat4ProjectionLeft;


		renderer.renderToDisplay();
		renderer.updateDisplay();
	}
}

void loadRoom(IRenderer* renderer) {

	
}

void SpaceWarpVisualization() {
	/*
	Interestingly, the following does not work. I think it tries to use a copy
	constructor because the compiler thinks the types are technically not equal.
	Renderer<GLRenderer> renderer = GLRenderer();
	This is the fix:
	GLRenderer renderer = GLRenderer();
	*/
	//SDLDisplay display(1800, 1000, "hello sdl");
	OpenVRDisplay display(1920, 1080, "hello sdl");
	GLRenderer renderer(&display);
	renderer.init();
	// HACK: use a glm projection matrix among Valve Matrices because I don't want to write my own projection matrix code.
	glm::mat4 proj = glm::perspective(3.14159f * .5f, display.getWidth() / (float)display.getHeight(), .1f, 100.f);
	renderer.projection = ((Matrix4*)&proj)[0];
	//renderer.view.rotateX(-45.0f);
	renderer.view.translate(Vector3(0, -1, -2));

	// Room
	
	GLShader phongVertShader(VERTEX_SHADER);
	phongVertShader.loadSource(readFile("phong.vs"));
	GLShader phongFragShader(FRAGMENT_SHADER);
	phongFragShader.loadSource(readFile("phong.fs"));
	GLProgram phongProgram(&phongVertShader, &phongFragShader);
	phongProgram.link();

	IMaterial whiteMaterial(&phongProgram);
	whiteMaterial.setFloat4("color", .9f, .9f, .9f, 1);
	IMaterial blackMaterial(&phongProgram);
	blackMaterial.setFloat4("color", .2f, .2f, .2f, 1);
	IMaterial brownMaterial(&phongProgram);
	brownMaterial.setFloat4("color", .6f, .4f, .2f, 1);
	IMaterial redMaterial(&phongProgram);
	redMaterial.setFloat4("color", .9f, 0, 0, 1);

	int W = 39; //odd
	float squareWidth = .1;
	Matrix4 placer;
	placer.scale(squareWidth / 2.0f).rotateX(-90);
	placer.translate(-W * squareWidth / 2, 0, -W * squareWidth / 2);
	GLModel whitefloor, blackfloor;
	bool white = true;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < W; j++) {
			if (white)
				whitefloor.addPlane(1, placer);
			else
				blackfloor.addPlane(1, placer);
			white = !white;
			placer.translate(squareWidth, 0, 0);
		}
		placer.translate(-squareWidth * W, 0, squareWidth);
	}
	whitefloor.loadBuffers();
	blackfloor.loadBuffers();
	renderer.addModel(&whitefloor);
	renderer.addModel(&blackfloor);
	whitefloor.setMaterial(&whiteMaterial);
	blackfloor.setMaterial(&blackMaterial);

	placer.identity().translate(0, 0, W * squareWidth / 2 + 3);
	GLModel walls[4];
	for (int i = 0; i < 4; i++) {
		walls[i].addCube(3);
		walls[i].loadBuffers();
		walls[i].setMaterial(&brownMaterial);
		walls[i].transform = placer;
		//walls[i].transform = walls[i].transform
		renderer.addModel(&walls[i]);
		placer.rotateY(90);
	}

	GLModel ceiling;
	ceiling.addCube(3);
	ceiling.loadBuffers();
	ceiling.setMaterial(&whiteMaterial);
	ceiling.transform = Matrix4().translate(0, 2 + 3, 0);
	renderer.addModel(&ceiling);

	const int P = 5;
	GLModel balls[P];
	float ballWidth = .2f;
	float spaceWidth = .4f;
	placer.identity().translate(-(ballWidth + spaceWidth) * P / 2, ballWidth / 2, -.5f);
	for (int i = 0; i < P; i++) {
		balls[i].addSphere(ballWidth / 2);
		balls[i].loadBuffers();
		balls[i].setMaterial(&redMaterial);
		balls[i].transform = placer;
		renderer.addModel(&balls[i]);
		placer.translate(ballWidth + spaceWidth, 0, 0);
	}

	const int K = 8;
	GLModel boxes[K];
	float boxWidth = .15f;
	spaceWidth = .3f;
	placer.identity().translate(-(boxWidth + spaceWidth) * K / 2, boxWidth / 2, .5f);
	for (int i = 0; i < K; i++) {
		boxes[i].addCube(boxWidth / 2);
		boxes[i].loadBuffers();
		boxes[i].setMaterial(&brownMaterial);
		boxes[i].transform = placer;
		renderer.addModel(&boxes[i]);
		placer.translate(boxWidth + spaceWidth, 0, 0);
	}

	// Loop

	long lastTime = SysTimeMS();
	float time = 0;
	while (!display.isClosed()) {
		long currTime = SysTimeMS();
		float deltaT = (currTime - lastTime) / 1000.f;
		if (deltaT < 0)
			lastTime = lastTime + 0;
		lastTime = currTime;

		//float time = currTime / 1000.f;
		time += 1.f / 90.f;
		whiteMaterial.setFloat("time", time);
		blackMaterial.setFloat("time", time);
		redMaterial.setFloat("time", time);
		brownMaterial.setFloat("time", time);


		renderer.view = display.GetUpdatedHMDMatrixPose();
		renderer.view = renderer.view * Matrix4().translate(0, -.1, 0);
		//renderer.projection = display.m_mat4ProjectionLeft;


		renderer.renderToDisplay();
		renderer.updateDisplay();
	}
}

int main(int argc, char *argv[])
{
	//GFieldVisualization();
	SpaceWarpVisualization();

	return 0;
}

