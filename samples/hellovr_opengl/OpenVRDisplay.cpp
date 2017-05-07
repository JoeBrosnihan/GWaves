
#include "OpenVRDisplay.h"

#include <iostream>
#include <GL/glew.h>
#include "glm/ext.hpp"


// Contains a lot of code from Valve OpenVR sample

//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}

Matrix4 OpenVRDisplay::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!m_pHMD)
		return Matrix4();

	vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(nEye, m_fNearClip, m_fFarClip);

	return Matrix4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

Matrix4 OpenVRDisplay::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!m_pHMD)
		return Matrix4();

	vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(nEye);
	Matrix4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return matrixObj.invert();
}

Matrix4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	Matrix4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}

void OpenVRDisplay::updateHMDMatrixPose()
{
	if (!m_pHMD)
		hmdCam.setView(Matrix4());

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		Matrix4 hmdView = ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
		//hmdView.translate(-1, -1, -1.5f);
		hmdView.rotateY(-90);
		hmdView.translate(1.5f, -1, -1);
		hmdView.invert();
		hmdCam.setView(hmdView);
		leftEyeCam.setView(m_mat4eyePosLeft * hmdView);
		rightEyeCam.setView(m_mat4eyePosRight * hmdView);
	}
}

OpenVRDisplay::OpenVRDisplay(int width, int height, const std::string &title) : IDisplay(width, height, title),
	m_bVblank(false), m_fNearClip(.01f), m_fFarClip(1000)
{
	SDL_Init(SDL_INIT_EVERYTHING); // assumes only one window for lifetime of program

	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL);
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (window == NULL)
		printf("%s - Window could not be created! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL)
		printf("%s - OpenGL context could not be created! SDL Error: %s\n", __FUNCTION__, SDL_GetError());

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "Glew failed to initialize!\n";
		exit(EXIT_FAILURE);
	}
	glGetError(); // clear any error

	if (SDL_GL_SetSwapInterval(m_bVblank ? 1 : 0) < 0)
		printf("%s - Warning: Unable to set VSync! SDL Error: %s\n", __FUNCTION__, SDL_GetError());


	m_strDriver = "No Driver";
	m_strDisplay = "No Display";

	m_strDriver = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	m_strDisplay = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	// Setup Cameras
	leftEyeCam.setProjection(GetHMDMatrixProjectionEye(vr::Eye_Left));
	rightEyeCam.setProjection(GetHMDMatrixProjectionEye(vr::Eye_Right));
	m_mat4eyePosLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
	m_mat4eyePosRight = GetHMDMatrixPoseEye(vr::Eye_Right);
	hmdCam.setProjection(90, getWidth() / (float)getHeight(), .01f, 100.f);

	// Setup Render Targets
	if (!m_pHMD)
		printf("NO HMD!\n");

	uint32_t recommendedWidth, recommendedHeight;
	m_pHMD->GetRecommendedRenderTargetSize(&recommendedWidth, &recommendedHeight);

	leftEyeTex = GLTexture(recommendedWidth, recommendedHeight, RGBA, nullptr);
	leftEyeRT = GLRenderTarget(&leftEyeTex);
	rightEyeTex = GLTexture(recommendedWidth, recommendedHeight, RGBA, nullptr);
	rightEyeRT = GLRenderTarget(&rightEyeTex);

	// Setup Companion Window
		// TODO later

	// Skip Setup Render Models
	
	// Init Compositor
	if (!vr::VRCompositor())
		printf("Compositor initialization failed. See log file for details\n");

	SDL_StartTextInput();
	//SDL_ShowCursor(SDL_DISABLE);
}

void OpenVRDisplay::update() {
	vr::Texture_t leftEyeTexture;
	leftEyeTexture.handle = (void*)leftEyeTex.getTextureObject();
	leftEyeTexture.eType = vr::TextureType_OpenGL;
	leftEyeTexture.eColorSpace = vr::ColorSpace_Gamma;
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);

	vr::Texture_t rightEyeTexture;
	rightEyeTexture.handle = (void*)rightEyeTex.getTextureObject();
	rightEyeTexture.eType = vr::TextureType_OpenGL;
	rightEyeTexture.eColorSpace = vr::ColorSpace_Gamma;
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

	SDL_GL_SwapWindow(window);
}

void OpenVRDisplay::handleInput()
{
	updateHMDMatrixPose();

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		if (sdlEvent.type == SDL_QUIT)
		{
			closed = true;
		}
		else if (sdlEvent.type == SDL_KEYDOWN)
		{
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE
				|| sdlEvent.key.keysym.sym == SDLK_q)
			{
				closed = true;
			}
		}
	}
}

OpenVRDisplay::~OpenVRDisplay() {
	SDL_StopTextInput();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
