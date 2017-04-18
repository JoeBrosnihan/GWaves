//========= Copyright Valve Corporation ============//

#include <SDL.h>

#include <iostream>

#include "SDLDisplay.h"
#include "GLRenderer.h"
#include "GLRenderTarget.h"
#include "IOUtils.h"
#include "FluidSim.h"

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

struct Vertex { // just for fluid sim
public:
	Vertex(float x, float y, float z, float u, float v)
		: x(x), y(y), z(z), u(u), v(v) {};
private:
	float x, y, z, u, v;
};

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
	
	GLRenderTarget target(300, 200, 4);
	GLModel cubes;
	cubes.cubeTest();
	cubes.loadBuffers();
	renderer.addModel(&cubes);

	//BEGIN FLUID SIM CODEBLOCK

	GLuint vao, vbo;
	Vertex verts[] = {
		Vertex(-.5, .5, 0, 0, 0),
		Vertex(.5, .5, 0, 1, 0),
		Vertex(-.5, -.5, 0, 0, 1),
		Vertex(-.5, -.5, 0, 0, 1),
		Vertex(.5, .5, 0, 1, 0),
		Vertex(.5, -.5, 0, 1, 1),
	};

	//Vertex Array

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(3 * sizeof(float)));

	glBindVertexArray(0);

	//Shader Program

	if (false) {
		GLuint program = glCreateProgram();

		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		std::string vertSource = readFile("fluid.vs");
		GLint vertSourceLen = vertSource.length();
		GLchar* vertSourceC = (GLchar *)vertSource.c_str();
		glShaderSource(vertShader, 1, &vertSourceC, &vertSourceLen);
		glCompileShader(vertShader);

		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::string fragSource = readFile("fluid.fs");
		GLint fragSourceLen = fragSource.length();
		GLchar* fragSourceC = (GLchar *)fragSource.c_str();
		glShaderSource(fragShader, 1, &fragSourceC, &fragSourceLen);
		glCompileShader(fragShader);

		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		glBindAttribLocation(program, 0, "postion");
		glLinkProgram(program);
		glValidateProgram(program); //what does this do???

		GLuint texUniformHandle = glGetUniformLocation(program, "texture");
	}

	// Material

	GLShader vertShader(VERTEX_SHADER);
	vertShader.loadSource(readFile("fluid.vs"));

	GLShader fragShader(FRAGMENT_SHADER);
	fragShader.loadSource(readFile("fluid.fs"));

	GLProgram fluidProgram(&vertShader, &fragShader);
	fluidProgram.link();

	IMaterial fluidMaterial(&fluidProgram);

	//Texture

	const int TEX_SIZE = 32;
	char texData[TEX_SIZE * TEX_SIZE * 4];
	for (int i = 0; i < TEX_SIZE * TEX_SIZE; i++) {
		texData[i] = (i % 256);
	}
	GLTexture texture(TEX_SIZE, TEX_SIZE, texData);
	fluidMaterial.setTexture("texture", &texture);

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

	// END FLUID SIM CODEBLOCK

	while (!display.isClosed()) {
		renderer.renderFrame();
		/*
		// FLUID SIM CODEBLOCK

		glUseProgram(program);

		//Use texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(texUniformHandle, 0);
		*/

		fluidMaterial.useMaterial();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(verts) / sizeof(verts[0]));

		// END FLUID SIM CODEBLOCK

		renderer.updateDisplay();

		updateFluid(fluid, texture, texData, TEX_SIZE);
	}

	return 0;
}
