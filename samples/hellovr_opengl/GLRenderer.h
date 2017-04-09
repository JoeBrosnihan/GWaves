#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <iostream>

#include "GLRenderTarget.h"
#include "GLModel.h"

#include "IRenderer.h"

class GLRenderer;

typedef IRenderer<GLRenderer, GLRenderTarget, GLModel> GLRendererParent; //if this causes problems, note that it is not necessary. I did this for convenience

class GLRenderer : public GLRendererParent {
public:
	GLRenderer(IDisplay * display);
	void init();
	void renderFrame();
private: // SDL bookkeeping
	SDL_Window *m_pCompanionWindow;
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;

	SDL_GLContext m_pContext;
private: // GL bookkeeping
	GLuint m_unSceneProgramID;
	GLuint m_nSceneMatrixLocation;
	GLuint m_nTimeLocation;
	GLuint m_unCompanionWindowProgramID;
private:
	bool initGL();
	GLuint compileShader(const char *shaderName, const char *vertexShader, const char *fragmentShader);
};
