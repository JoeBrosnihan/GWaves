#pragma once

#include <vector>

class FluidSim {
public:
	FluidSim(int size);
	void stepVel(float dt, float viscosity);
	void stepDens(float dt, float diffuse);
	std::vector<float> u, v, p;
private:
	int N;
	std::vector<float> u0, v0, p0;
	inline int IX(int i, int j) {
		return i + N * j;
	};
};
