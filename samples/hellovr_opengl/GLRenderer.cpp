
#include "GLRenderer.h"

#include <stdio.h>
#include <string>

#include "IOUtils.h"

GLRenderer::GLRenderer(IDisplay * display)
	: IRenderer(display)
	//, m_pCompanionWindow(NULL)
	//, m_pContext(NULL)
	//, m_nCompanionWindowWidth(640)
	//, m_nCompanionWindowHeight(320)
	, m_unSceneProgramID(0)
	, m_nSceneMatrixLocation(-1)
	, m_unCompanionWindowProgramID(0)
{
	int a = 3;
}

void GLRenderer::init() {
	/* The following is taken care of in the IDipslay creation.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("%s - SDL could not initialize! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
		exit(-1);
	}

	int nWindowPosX = 700;
	int nWindowPosY = 100;
	Uint32 unWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

	m_pCompanionWindow = SDL_CreateWindow("hellovr", nWindowPosX, nWindowPosY, m_nCompanionWindowWidth, m_nCompanionWindowHeight, unWindowFlags);
	if (m_pCompanionWindow == NULL)
	{
		printf("%s - Window could not be created! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
		exit(-1);
	}

	m_pContext = SDL_GL_CreateContext(m_pCompanionWindow);
	if (m_pContext == NULL)
	{
		printf("%s - OpenGL context could not be created! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
		exit(-1);
	}

	glewExperimental = GL_TRUE;
	GLenum nGlewError = glewInit();
	if (nGlewError != GLEW_OK)
	{
		printf("%s - Error initializing GLEW! %s\n", __FUNCTION__, glewGetErrorString(nGlewError));
		exit(-1);
	}
	glGetError(); // to clear the error caused deep in GLEW
	*/

	if (!initGL())
	{
		printf("%s - Unable to initialize OpenGL!\n", __FUNCTION__);
		exit(-1);
	}

	/* Is this necessary? Does it belong here or in the display creation?
	if (SDL_GL_SetSwapInterval(0) < 0)
	{
		printf("%s - Warning: Unable to set VSync! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
		exit(-1);
	}
	*/
}

bool GLRenderer::initGL() {
	m_unSceneProgramID = compileShader(
		"Scene",

		// Vertex Shader
		readFile("bare.vs").c_str(),


		// Fragment Shader
		readFile("bare.fs").c_str()
		);
	/*m_nSceneMatrixLocation = glGetUniformLocation(m_unSceneProgramID, "matrix");
	if (m_nSceneMatrixLocation == -1)
	{
		dprintf("Unable to find matrix uniform in scene shader\n");
		return false;
	}
	m_nTimeLocation = glGetUniformLocation(m_unSceneProgramID, "time");
	if (m_nTimeLocation == -1)
	{
		dprintf("Unable to find time uniform in scene shader\n");
		return false;
	}*/

	m_unCompanionWindowProgramID = compileShader(
		"CompanionWindow",

		// vertex shader
		readFile("companion_window.vs").c_str(),

		// fragment shadershader
		readFile("companion_window.fs").c_str()
		);

	if (!m_unSceneProgramID || !m_unCompanionWindowProgramID)
		return false;

	return true;
}

GLuint GLRenderer::compileShader(const char *shaderName, const char *vertexShader, const char *fragmentShader)
{
	GLuint unProgramID = glCreateProgram();

	GLuint nSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(nSceneVertexShader, 1, &vertexShader, NULL);
	glCompileShader(nSceneVertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneVertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		dprintf("%s - Unable to compile vertex shader %d!\n", shaderName, nSceneVertexShader);

		GLchar infoLog[5000] = { 0 };
		GLint infoLogLength;
		glGetShaderInfoLog(nSceneVertexShader, sizeof(infoLog) - 1, &infoLogLength, infoLog);
		dprintf("%s\n", infoLog);

		glDeleteProgram(unProgramID);
		glDeleteShader(nSceneVertexShader);
		return 0;
	}
	glAttachShader(unProgramID, nSceneVertexShader);
	glDeleteShader(nSceneVertexShader); // the program hangs onto this once it's attached

	GLuint  nSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nSceneFragmentShader, 1, &fragmentShader, NULL);
	glCompileShader(nSceneFragmentShader);

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneFragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		dprintf("%s - Unable to compile fragment shader %d!\n", shaderName, nSceneFragmentShader);

		GLchar infoLog[5000] = { 0 };
		GLint infoLogLength;
		glGetShaderInfoLog(nSceneFragmentShader, sizeof(infoLog) - 1, &infoLogLength, infoLog);
		dprintf("%s\n", infoLog);

		glDeleteProgram(unProgramID);
		glDeleteShader(nSceneFragmentShader);
		return 0;
	}

	glAttachShader(unProgramID, nSceneFragmentShader);
	glDeleteShader(nSceneFragmentShader); // the program hangs onto this once it's attached

	glLinkProgram(unProgramID);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(unProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		dprintf("%s - Error linking program %d!\n", shaderName, unProgramID);
		glDeleteProgram(unProgramID);
		return 0;
	}

	glUseProgram(unProgramID);
	glUseProgram(0);

	return unProgramID;
}

void GLRenderer::renderToDisplay() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display->getWidth(), display->getHeight());
	render();
	glFinish();

	// We want to make sure the glFinish waits for the entire present to complete, not just the submission
	// of the command. So, we do a clear here right here so the glFinish will wait fully for the swap.
	//glClearColor(.5f, 0, 1, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::renderTo(IRenderTarget* target) {
	target->useTarget();
	render();
}

void GLRenderer::render() {
	static float flop = 0;
	flop = !flop * .25f;
	glClearColor(flop, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluPerspective(90.f, display->getWidth() / (float)display->getHeight(), 1.f, 1000.f);
	glEnableClientState(GL_VERTEX_ARRAY);

	renderScene();

	glDisable(GL_DEPTH_TEST);
}
