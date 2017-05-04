//========= Copyright Valve Corporation ============//

#include <SDL.h>

#include <iostream>

#include "GLRenderer.h"
#include "GLRenderTarget.h"
#include "IOUtils.h"
#include "IModel.h"
#include "GWaveSim.h"
#include "shared/lodepng.h"
#include "shared/pathtools.h"

#include "GLTexture.h"

#define VR

#ifdef VR
#include "OpenVRDisplay.h"
#else
#include "SDLDisplay.h"
#endif

void GFieldVisualization() {
	Matrix4 shift;
#ifndef VR
	SDLDisplay display(1800, 1000, "hello sdl");
#else
	OpenVRDisplay display(1920, 1080, "hello sdl");
#endif
	GLRenderer renderer(&display);

	// Gravity Field

	GLModel surface;
	const int RESOLUTION = 1024;
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

	// Black Holes

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
	holeMaterial.setFloat("time", 0);
	blackHole.setMaterial(&holeMaterial);
	renderer.addModel(&blackHole);
	blackHole2.setMaterial(&holeMaterial);
	renderer.addModel(&blackHole2);

	// Skybox

	GLModel skybox;
	//skybox.addCube(-1);
	skybox.addSphere(-1);
	skybox.loadBuffers();

	GLShader skyVertShader(VERTEX_SHADER);
	skyVertShader.loadSource(readFile("skybox.vs"));
	GLShader skyFragShader(FRAGMENT_SHADER);
	skyFragShader.loadSource(readFile("unlit.fs"));
	GLProgram skyProgram(&skyVertShader, &skyFragShader);
	skyProgram.link();

	std::vector<unsigned char> skyTexData;
	unsigned skyTexWidth, skyTexHeight;
	lodepng::decode(skyTexData, skyTexWidth, skyTexHeight, "sky.png");
	GLTexture skyTexture((int) skyTexWidth, (int) skyTexHeight, RGBA, (char*) skyTexData.data());

	IMaterial skyMaterial(&skyProgram);
	skyMaterial.setTexture("texture", &skyTexture);
	skybox.setMaterial(&skyMaterial);
	renderer.setSkybox(&skybox);

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

		display.handleInput();
		//companion window
#ifdef VR
		renderer.setCamera(display.getHMDCam());
#else
		renderer.setCamera(display.getCamera());
#endif
		renderer.renderToDisplay();
		//left eye
	//	renderer.setCamera(display.getLeftEyeCam());
	//	renderer.renderTo(display.getLeftEyeTarget());
		//right eye
	//	renderer.setCamera(display.getRightEyeCam());
	//	renderer.renderTo(display.getRightEyeTarget());
		//show all
		display.update();
	}
}

void SpaceWarpVisualization() {
#ifndef VR
	SDLDisplay display(1800, 1000, "hello sdl");
#else
	OpenVRDisplay display(1920, 1080, "hello sdl");
#endif
	GLRenderer renderer(&display);

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
	float roomSize = W * squareWidth;
	Matrix4 placer;
	placer.scale(squareWidth / 2.0f).rotateX(-90);
	placer.translate(-(W - 1) * squareWidth / 2, 0, -(W - 1) * squareWidth / 2);
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
		placer.translate(-roomSize, 0, squareWidth);
	}
	whitefloor.loadBuffers();
	blackfloor.loadBuffers();
	renderer.addModel(&whitefloor);
	renderer.addModel(&blackfloor);
	whitefloor.setMaterial(&whiteMaterial);
	blackfloor.setMaterial(&blackMaterial);

	placer.identity().scale(roomSize * .5f, roomSize * .5f, roomSize * .5f).translate(0, roomSize * .5f, roomSize * .5f);
	GLModel walls[4];
	for (int i = 0; i < 4; i++) {
		walls[i].addPlane(W, placer);
		walls[i].loadBuffers();
		walls[i].setMaterial(&brownMaterial);
		renderer.addModel(&walls[i]);
		placer.rotateY(90);
	}

	GLModel ceiling;
	ceiling.addPlane(W, Matrix4());
	ceiling.loadBuffers();
	ceiling.setMaterial(&whiteMaterial);
	ceiling.transform = Matrix4().scale(roomSize * .5f).rotateX(90).translate(0, roomSize * .5f, 0);
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


		//renderer.view = renderer.view * Matrix4().translate(0, -.1, 0);
		//renderer.projection = display.m_mat4ProjectionLeft;


		display.handleInput();
		//companion window
#ifdef VR
		renderer.setCamera(display.getHMDCam());
#else
		renderer.setCamera(display.getCamera());
#endif
		renderer.renderToDisplay();
		//left eye
		//	renderer.setCamera(display.getLeftEyeCam());
		//	renderer.renderTo(display.getLeftEyeTarget());
		//right eye
		//	renderer.setCamera(display.getRightEyeCam());
		//	renderer.renderTo(display.getRightEyeTarget());
		//show all
		display.update();
	}
}

int main(int argc, char *argv[])
{
	//GFieldVisualization();
	SpaceWarpVisualization();

	return 0;
}

