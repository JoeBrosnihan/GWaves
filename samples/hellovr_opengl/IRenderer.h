#pragma once

/*
Manages the window and rendering of our application.
This class follows the CRTP to avoid virtual function call overhead.
*/

#include "IDisplay.h"

template <typename Impl, typename RenderTarget, typename Model>
class IRenderer {
public:
	typedef RenderTarget RenderTarget;
	typedef Model Model;

	IRenderer(IDisplay * display) : display(display) {}

	// Adds a renderable model to the scene
	void addModel(Model* model) {
		//models.pushBack(model);
		m = model;
	}

	void init() {
		static_cast<Impl*>(this)->init();
	};

	void renderFrame() {
		static_cast<Impl*>(this)->renderFrame();
	};

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
	RenderTarget *rendertarget;
	IDisplay *display;

	//scene
	Model* m;
	//std::vector
};
