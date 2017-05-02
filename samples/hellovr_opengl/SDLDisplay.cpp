
#include "SDLDisplay.h"

#include <iostream>
#include <GL/glew.h>

SDLDisplay::SDLDisplay(int width, int height, const std::string &title) : IDisplay(width, height, title) {
	SDL_Init(SDL_INIT_EVERYTHING); // assumes only one window for lifetime of program

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "Glew failed to initialize!\n";
		exit(EXIT_FAILURE);
	}
	glGetError(); // clear any error

	camera.setProjection(90, getWidth() / (float)getHeight());
}

void SDLDisplay::update() {
	SDL_GL_SwapWindow(window);

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			closed = true;
	}
}

SDLDisplay::~SDLDisplay() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void SDLDisplay::handleInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		if (sdlEvent.type == SDL_QUIT)
		{
			closed = true;
		}
		else if (sdlEvent.type == SDL_KEYDOWN)
		{
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE
				|| sdlEvent.key.keysym.sym == SDLK_q)
			{
				closed = true;
			}
		}
	}
}
