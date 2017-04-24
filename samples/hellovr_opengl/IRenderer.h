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
	IRenderer(IDisplay* display) : display(display), view(), projection() {}

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

	Matrix4 view;
	Matrix4 projection;
protected:
	void renderScene() {
		for (auto it = models.begin(); it != models.end(); ++it) {
			(*it)->render(projection * view); // use camera matrix here
		}
	};
	IRenderTarget* rendertarget;
	IDisplay* display;

	//scene
	std::vector<IModel*> models;
};
