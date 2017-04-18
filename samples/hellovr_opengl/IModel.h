#pragma once

#include <vector>
#include "Shared/Matrices.h"
#include "Shared/Vectors.h"
#include "IMaterial.h"



class IModel {
public:
	struct Vertex {
		float x, y, z, nx, ny, nz, u, v;
		Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
			: x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v) {};
	};

	IModel() : vertices(), material(nullptr) {}; // Is vertices() necessary?

	void setMaterial(IMaterial* material) { this->material = material; }

	void addFace(Vertex a, Vertex b, Vertex c) {
		vertices.push_back(a);
		vertices.push_back(b);
		vertices.push_back(c);
	};

	//create test model
	void cubeTest() {
		for (int x = -5; x <= 5; x++) {
			for (int y = -5; y <= 5; y++) {
				for (int z = -5; z <= 5; z++) {
					addCube(Matrix4().translate(x*3.0f, y*3.0f, z*3.0f));
				}
			}
		}
	};

	virtual void render() = 0;
	virtual void loadBuffers() = 0;
	
protected:
	std::vector<Vertex> vertices;
	IMaterial* material;

private:
	void AddCubeVertex(float x, float y, float z, float u, float v, int unused) {
		vertices.push_back(Vertex(x, y, z, 0, 0, 0, 0, 0));
	};

	void addCube(Matrix4 &mat) {
		// Matrix4 mat( outermat.data() );

		Vector4 A = mat * Vector4(0, 0, 0, 1);
		Vector4 B = mat * Vector4(1, 0, 0, 1);
		Vector4 C = mat * Vector4(1, 1, 0, 1);
		Vector4 D = mat * Vector4(0, 1, 0, 1);
		Vector4 E = mat * Vector4(0, 0, 1, 1);
		Vector4 F = mat * Vector4(1, 0, 1, 1);
		Vector4 G = mat * Vector4(1, 1, 1, 1);
		Vector4 H = mat * Vector4(0, 1, 1, 1);

		int vertdata = 0; //unused
		// triangles instead of quads
		AddCubeVertex(E.x, E.y, E.z, 0, 1, vertdata); //Front
		AddCubeVertex(F.x, F.y, F.z, 1, 1, vertdata);
		AddCubeVertex(G.x, G.y, G.z, 1, 0, vertdata);
		AddCubeVertex(G.x, G.y, G.z, 1, 0, vertdata);
		AddCubeVertex(H.x, H.y, H.z, 0, 0, vertdata);
		AddCubeVertex(E.x, E.y, E.z, 0, 1, vertdata);

		AddCubeVertex(B.x, B.y, B.z, 0, 1, vertdata); //Back
		AddCubeVertex(A.x, A.y, A.z, 1, 1, vertdata);
		AddCubeVertex(D.x, D.y, D.z, 1, 0, vertdata);
		AddCubeVertex(D.x, D.y, D.z, 1, 0, vertdata);
		AddCubeVertex(C.x, C.y, C.z, 0, 0, vertdata);
		AddCubeVertex(B.x, B.y, B.z, 0, 1, vertdata);

		AddCubeVertex(H.x, H.y, H.z, 0, 1, vertdata); //Top
		AddCubeVertex(G.x, G.y, G.z, 1, 1, vertdata);
		AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
		AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
		AddCubeVertex(D.x, D.y, D.z, 0, 0, vertdata);
		AddCubeVertex(H.x, H.y, H.z, 0, 1, vertdata);

		AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata); //Bottom
		AddCubeVertex(B.x, B.y, B.z, 1, 1, vertdata);
		AddCubeVertex(F.x, F.y, F.z, 1, 0, vertdata);
		AddCubeVertex(F.x, F.y, F.z, 1, 0, vertdata);
		AddCubeVertex(E.x, E.y, E.z, 0, 0, vertdata);
		AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata);

		AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata); //Left
		AddCubeVertex(E.x, E.y, E.z, 1, 1, vertdata);
		AddCubeVertex(H.x, H.y, H.z, 1, 0, vertdata);
		AddCubeVertex(H.x, H.y, H.z, 1, 0, vertdata);
		AddCubeVertex(D.x, D.y, D.z, 0, 0, vertdata);
		AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata);

		AddCubeVertex(F.x, F.y, F.z, 0, 1, vertdata); //Right
		AddCubeVertex(B.x, B.y, B.z, 1, 1, vertdata);
		AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
		AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
		AddCubeVertex(G.x, G.y, G.z, 0, 0, vertdata);
		AddCubeVertex(F.x, F.y, F.z, 0, 1, vertdata);
	};
};
