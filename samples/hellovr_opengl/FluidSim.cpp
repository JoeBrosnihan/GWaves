
#include "FluidSim.h"

#define IX(i, j) ((i) + (N + 2) * (j))
#define SWAP(x, x0) {std::swap(x, x0);}

FluidSim::FluidSim(int N) : N(N) {
	int totSize = (N + 2) * (N + 2);
	u.reserve(totSize);
	u.insert(u.begin(), totSize, 0);
	v.reserve(totSize);
	v.insert(v.begin(), totSize, 0);
	p.reserve(totSize);
	p.insert(p.begin(), totSize, 0);
	u0.reserve(totSize);
	u0.insert(u0.begin(), totSize, 0);
	v0.reserve(totSize);
	v0.insert(v0.begin(), totSize, 0);
	p0.reserve(totSize);
	p0.insert(p0.begin(), totSize, 0);
}

void set_bnd(int N, int b, std::vector<float> &x)
{
	for (int i = 1; i <= N; i++) {
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
		x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
		x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
	}
	x[IX(0, 0)] = 0.5*(x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5*(x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5*(x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5*(x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void diffuse(int N, int b, std::vector<float> &x, std::vector<float> &x0, float diff, float dt)
{
	int i, j, k;
	float a = dt*diff*N*N;
	for (k = 0; k<20; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				int index = IX(i, j);
				x[IX(i, j)] = (x0[IX(i, j)] + a*(x[IX(i - 1, j)] + x[IX(i + 1, j)] +
					x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
		set_bnd(N, b, x);
	}
}

void advect(int N, int b, std::vector<float> &d, std::vector<float> &d0, std::vector<float> &u, std::vector<float> &v, float dt)
{
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;
	dt0 = dt*N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			x = i - dt0*u[IX(i, j)]; y = j - dt0*v[IX(i, j)];
			if (x<0.5) x = 0.5; if (x>N + 0.5) x = N + 0.5; i0 = (int)x; i1 = i0 + 1;
			if (y<0.5) y = 0.5; if (y>N + 0.5) y = N + 0.5; j0 = (int)y; j1 = j0 + 1;
			s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1 - t1;
			d[IX(i, j)] = s0*(t0*d0[IX(i0, j0)] + t1*d0[IX(i0, j1)]) +
				s1*(t0*d0[IX(i1, j0)] + t1*d0[IX(i1, j1)]);
		}
	}
	set_bnd(N, b, d);
}

void project(int N, std::vector<float> &u, std::vector<float> &v, std::vector<float> &p, std::vector<float> &div)
{
	int i, j, k;
	float h;
	h = 1.0 / N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			div[IX(i, j)] = -0.5*h*(u[IX(i + 1, j)] - u[IX(i - 1, j)] +
				v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0;
		}
	}
	set_bnd(N, 0, div); set_bnd(N, 0, p);
	for (k = 0; k < 20; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			}
		}
		set_bnd(N, 0, p);
	}
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			u[IX(i, j)] -= 0.5*(p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5*(p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}
	set_bnd(N, 1, u); set_bnd(N, 2, v);
}

void FluidSim::stepVel(float dt, float viscosity) {
	SWAP(u, u0);
	diffuse(N, 1, u, u0, viscosity, dt);
	SWAP(v, v0);
	diffuse(N, 2, v, v0, viscosity, dt);
	project(N, u, v, u0, v0);
	SWAP(u, u0);
	SWAP(v, v0);
	advect(N, 1, u, u0, u0, v0, dt);
	advect(N, 2, v, v0, u0, v0, dt);
	project(N, u, v, u0, v0);
}

void FluidSim::stepDens(float dt, float diff) {
	SWAP(p, p0);
	diffuse(N, 0, p, p0, diff, dt);
	SWAP(p, p0);
	advect(N, 0, p, p0, u, v, dt);
}
