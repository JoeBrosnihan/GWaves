#pragma once

#include <SDL.h>
#include <GL/glew.h>

#include "IRenderer.h"

class GLRenderer : public IRenderer<GLRenderer> {
public:
	GLRenderer();
	void init();
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
