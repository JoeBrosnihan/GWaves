#pragma once

/*
 * A Renderer has the job of rendering scenes. This specifically entails
 * rendering from the point of view of a camera a collection of models in
 * world space utilizing the depth buffer.
*/

#include "IDisplay.h"
#include "IModel.h"
#include "IRenderTarget.h"
#include "Camera.h"

class IRenderer {
public:
	IRenderer(IDisplay* display) : display(display), camera(nullptr) {}

	// Adds a renderable model to the scene
	void addModel(IModel* model) {
		models.push_back(model);
	}

	void setCamera(const Camera* camera)
	{
		this->camera = camera;
	}

	virtual void init() = 0;
	virtual void renderToDisplay() = 0;
	virtual void renderTo(const IRenderTarget* target) = 0;
protected:
	void renderScene() {
		Matrix4 vpMatrix;
		if (camera)
			vpMatrix = camera->getVPMatrix();
		for (auto it = models.begin(); it != models.end(); ++it) {
			(*it)->render(vpMatrix); // use camera matrix here
		}
	};
	IRenderTarget* rendertarget;
	IDisplay* display;
	const Camera* camera;

	//scene
	std::vector<IModel*> models;
};
