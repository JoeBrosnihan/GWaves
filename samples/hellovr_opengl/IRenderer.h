#pragma once

/*
Manages the window and rendering of our application.
This class follows the CRTP to avoid virtual function call overhead.
*/

#include "IDisplay.h"

class IRenderer {
public:
	IRenderer(IDisplay* display) : display(display) {}

	// Adds a renderable model to the scene
	void addModel(IModel* model) {
		//models.pushBack(model);
		m = model;
	}

	virtual void init() = 0;
	virtual void renderFrame() = 0;

	void updateDisplay() {
		display->update();
	}
protected:
	void renderScene() {
		//m->render();
		//for (auto it = models.begin(); it != models.end; ++it) {
		//	*it->render();
		//}
	};
	IRenderTarget* rendertarget;
	IDisplay* display;

	//scene
	IModel* m;
	//std::vector
};
