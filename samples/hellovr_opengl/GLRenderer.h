#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <iostream>

#include "GLRenderTarget.h"
#include "GLModel.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "IMaterial.h"

#include "IRenderer.h"


class GLRenderer : public IRenderer {
public:
	GLRenderer(IDisplay * display);
	virtual void init();
	virtual void renderToDisplay();
	virtual void renderTo(IRenderTarget* target);
	/*
private: // SDL bookkeeping
	SDL_Window *m_pCompanionWindow;
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;
	
	SDL_GLContext m_pContext; //obsolete! remove!
	*/
private: // GL bookkeeping
	GLuint m_unSceneProgramID;
	GLuint m_nSceneMatrixLocation;
	GLuint m_nTimeLocation;
	GLuint m_unCompanionWindowProgramID;
private:
	bool initGL();
	GLuint compileShader(const char *shaderName, const char *vertexShader, const char *fragmentShader);
	void render();
};
