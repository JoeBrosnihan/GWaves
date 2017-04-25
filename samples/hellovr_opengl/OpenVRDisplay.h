#pragma once

#include "IDisplay.h"

#include <SDL.h>
#include <openvr.h>
#include "shared/Matrices.h"
#include "GLRenderTarget.h"


static const uint32_t k_unMaxTrackedDeviceCount = 16;

class OpenVRDisplay : public IDisplay {
public:
	OpenVRDisplay(int width, int height, const std::string &title);
	virtual void update();
	virtual ~OpenVRDisplay();

	Matrix4 GetUpdatedHMDMatrixPose();

	Matrix4 m_mat4ProjectionLeft;
private:
	SDL_Window *window;
	SDL_GLContext glContext;

	GLTexture leftEyeTex;
	GLRenderTarget leftEyeRT;
	GLTexture rightEyeTex;
	GLRenderTarget rightEyeRT;



	Matrix4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	Matrix4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);

	vr::IVRSystem *m_pHMD;

	bool m_bVblank;
	float m_fNearClip;
	float m_fFarClip;

	std::string m_strDriver;
	std::string m_strDisplay;

	Matrix4 m_mat4HMDPose;
	Matrix4 m_mat4eyePosRight;
	Matrix4 m_mat4eyePosLeft;
	Matrix4 m_mat4ProjectionRight;

	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
};
