
#include "GLRenderer.h"

#include <stdio.h>
#include <string>

#include "IOUtils.h"


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

void GLRenderer::renderTo(const IRenderTarget* target) {
	target->useTarget();
	render();
}

void GLRenderer::render() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (skybox != nullptr) {
		Matrix4 vp;
		if (camera != nullptr) {
			Vector3 pos = camera->getPosition();
			vp = camera->getVPMatrix() * Matrix4().translate(camera->getPosition());
		}
		skybox->render(vp);
		//skybox->render(camera ? Matrix4().translate(camera->getPosition()) : Matrix4());
	}

	glEnable(GL_DEPTH_TEST);

	renderScene();

	glDisable(GL_DEPTH_TEST);
}
