#pragma once

/*
Manages the window and rendering of our application.
This class follows the CRTP to avoid virtual function call overhead.
*/

#include "IDisplay.h"
#include "IModel.h"
#include "IRenderTarget.h"

class IRenderer {
public:
	IRenderer(IDisplay* display) : display(display) {}

	// Adds a renderable model to the scene
	void addModel(IModel* model) {
		models.push_back(model);
	}

	virtual void init() = 0;
	virtual void renderToDisplay() = 0;
	virtual void renderTo(IRenderTarget* target) = 0;

	void updateDisplay() {
		display->update();
	}
protected:
	void renderScene() {
		for (auto it = models.begin(); it != models.end(); ++it) {
			(*it)->render(Matrix4()); // use camera matrix here
		}
	};
	IRenderTarget* rendertarget;
	IDisplay* display;

	//scene
	std::vector<IModel*> models;
};
