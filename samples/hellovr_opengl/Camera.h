#pragma once
#include "Shared/Matrices.h"

class Camera
{
	Matrix4 view;
	Matrix4 projection;
public:
	void setView(Matrix4 view) { this->view = view; }
	void setProjection(Matrix4 projection) { this->projection = projection; }
	Matrix4 getVPMatrix() const { return projection * view; }
};
