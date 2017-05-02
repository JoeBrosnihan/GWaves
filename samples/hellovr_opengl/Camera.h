#pragma once
#include "Shared/Matrices.h"
#include "glm/ext.hpp"

class Camera
{
	Matrix4 view;
	Matrix4 projection;
public:
	void setView(Matrix4 view) { this->view = view; }
	void setProjection(Matrix4 projection) { this->projection = projection; }
	void setProjection(float fov, float aspectRatio, float near = .1f, float far = 100.0f) {
		glm::mat4 proj = glm::perspective(3.14159f * fov / 180, aspectRatio, near, far);
		projection = ((Matrix4*)&proj)[0];
	}
	Matrix4 getVPMatrix() const { return projection * view; }
	Vector3 getPosition() const {
		Matrix4 inv = view;
		inv.invertGeneral();
		return Vector3(inv[12], inv[13], inv[14]);
	}
};
