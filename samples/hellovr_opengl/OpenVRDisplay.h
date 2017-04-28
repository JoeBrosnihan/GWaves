#pragma once

#include "IDisplay.h"

#include <SDL.h>
#include <openvr.h>
#include "shared/Matrices.h"
#include "GLRenderTarget.h"
#include "Camera.h"


static const uint32_t k_unMaxTrackedDeviceCount = 16;

class OpenVRDisplay : public IDisplay {
public:
	OpenVRDisplay(int width, int height, const std::string &title);
	virtual void update();
	virtual ~OpenVRDisplay();
	virtual void handleInput();

	const Camera* getHMDCam() { return &hmdCam; }
	const Camera* getLeftEyeCam() { return &leftEyeCam; }
	const Camera* getRightEyeCam() { return &rightEyeCam; }
	const GLRenderTarget* getLeftEyeTarget() { return &leftEyeRT; }
	const GLRenderTarget* getRightEyeTarget() { return &rightEyeRT; }
private:
	SDL_Window *window;
	SDL_GLContext glContext;

	Camera hmdCam;
	Camera leftEyeCam;
	GLTexture leftEyeTex;
	GLRenderTarget leftEyeRT;
	Camera rightEyeCam;
	GLTexture rightEyeTex;
	GLRenderTarget rightEyeRT;



	void updateHMDMatrixPose();
	Matrix4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	Matrix4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);

	vr::IVRSystem *m_pHMD;

	bool m_bVblank;
	float m_fNearClip;
	float m_fFarClip;

	std::string m_strDriver;
	std::string m_strDisplay;

	Matrix4 m_mat4eyePosRight;
	Matrix4 m_mat4eyePosLeft;

	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
};
