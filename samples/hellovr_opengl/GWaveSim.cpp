
#include "GWaveSim.h"

#include "IOUtils.h"
#include <math.h>

GWaveSim::GWaveSim(int size, GLProgram* displayProgram) : fieldVertShader(VERTEX_SHADER), fieldFragShader(FRAGMENT_SHADER), blurFragShader(FRAGMENT_SHADER), bodyFragShader(FRAGMENT_SHADER) {
	TEX_SIZE = size;

	float* fieldData = new float[TEX_SIZE * TEX_SIZE];
	for (int i = 0; i < TEX_SIZE * TEX_SIZE; i++) {
		fieldData[i] = 0;
	}

	for (int i = 0; i <= 1; i++)
	{
		fieldT[i] = GLTexture(TEX_SIZE, TEX_SIZE, R32F, (char*) fieldData);
		field[i] = GLRenderTarget(&fieldT[i]);
	}

	internalQuad = GLModel();
	internalQuad.addFace(
		IModel::Vertex(-1, 1, 0, 0, 0, 0, 0, 1),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0));
	internalQuad.addFace(
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(1, -1, 0, 0, 0, 0, 1, 0));
	internalQuad.loadBuffers();

	body = GLModel();
	body.addFace(
		IModel::Vertex(-1, 1, 0, 0, 0, 0, 0, 1),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0));
	body.addFace(
		IModel::Vertex(-1, -1, 0, 0, 0, 0, 0, 0),
		IModel::Vertex(1, 1, 0, 0, 0, 0, 1, 1),
		IModel::Vertex(1, -1, 0, 0, 0, 0, 1, 0));
	body.loadBuffers();

	displayMaterial = IMaterial(displayProgram);
	// this is a perfect example of why it is bad style to put GL initialization in a class constructor.
	//displayMaterial.setTexture("texture", (const ITexture*) &pt[1]);
	displayMaterial.setTexture("texture", field[0].getTexture());

	fieldVertShader.loadSource(readFile("fluid/diffuse.vs"));
	fieldFragShader.loadSource(readFile("fluid/gmedium.fs"));
	fieldProgram = GLProgram(&fieldVertShader, &fieldFragShader);
	fieldProgram.link();
	fieldMaterial = IMaterial(&fieldProgram);

	blurFragShader.loadSource(readFile("fluid/blur.fs"));
	blurProgram = GLProgram(&fieldVertShader, &blurFragShader);
	blurProgram.link();
	blurMaterial = IMaterial(&blurProgram);
	blurMaterial.setFloat2("pixelMarchVector", 0, 1.0f / TEX_SIZE);
	displayMaterial.setFloat2("pixelMarchVector", 1.0f / TEX_SIZE, 0);

	bodyFragShader.loadSource(readFile("fluid/body.fs"));
	bodyProgram = GLProgram(&fieldVertShader, &bodyFragShader);
	bodyProgram.link();
	bodyMaterial = IMaterial(&bodyProgram);
	bodyMaterial.setFloat("force", 1);
	body.setMaterial(&bodyMaterial);

	delete[] fieldData;
}

template<typename T>
void swap(T* p)
{
	T temp = p[0];
	p[0] = p[1];
	p[1] = temp;
}

void GWaveSim::step(IRenderer* renderer, float dt) {
	Matrix4 identity;
	swap(field);
	internalQuad.setMaterial(&fieldMaterial);
	fieldMaterial.setFloat("jump", dt * .5f);
	fieldMaterial.setFloat2("center", .5f, .5f);
	fieldMaterial.setTexture("g_field", field[1].getTexture());
	field[0].useTarget();
	internalQuad.render(identity);

	float time = SysTimeMS() / 1000.0f;
	float r = .05f, w = 5.0f;
	body.transform = Matrix4().scale(.025f).translate(Vector3(r * cos(time * w), r * sin(time * w), 0));
	body.render(identity);

	body.transform = Matrix4().scale(.025f).translate(Vector3(-r * cos(time * w), -r * sin(time * w), 0));
	body.render(identity);

	internalQuad.setMaterial(&blurMaterial);
	field[1].useTarget();
	blurMaterial.setTexture("source", field[0].getTexture());
	internalQuad.render(identity);

	displayMaterial.setTexture("texture", field[1].getTexture());
}
