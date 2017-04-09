#pragma once

#include "IDisplay.h"

#include <SDL.h>

class SDLDisplay : public IDisplay {
public:
	SDLDisplay(int width, int height, const std::string &title);
	virtual void update();
	virtual ~SDLDisplay();
private:
	SDL_Window *window;
	SDL_GLContext glContext;
};
