#pragma once

#include "IDisplay.h"
#include "Camera.h"

#include <SDL.h>

class SDLDisplay : public IDisplay {
public:
	SDLDisplay(int width, int height, const std::string &title);
	virtual void update();
	virtual ~SDLDisplay();
	virtual void handleInput();

	Camera* getCamera() { return &camera; }
private:
	SDL_Window *window;
	SDL_GLContext glContext;
	Camera camera;
};
