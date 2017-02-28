//========= Copyright Valve Corporation ============//

#include <SDL.h>

#include <iostream>

#include "IRenderer.h"
#include "GLRenderer.h"
#include "GLRenderTarget.h"

// Use the OpenGL implementation.
typedef GLRenderer Renderer;

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
	Renderer renderer;
	renderer.init();

	Renderer::RenderTarget target(300, 200, 4);

	bool bQuit = false;

	SDL_StartTextInput();
	SDL_ShowCursor(SDL_DISABLE);

	while (!bQuit)
	{
		bQuit = handleInput();

		renderer.renderFrame();
	}

	SDL_StopTextInput();

	return 0;
}
