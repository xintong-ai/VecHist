#ifndef GLYPH_RENDERABLE_H
#define GLYPH_RENDERABLE_H

#include "Renderable.h"
#include <QObject>

class Cubemap;
//class GLTextureCube;
class ShaderProgram;
class QOpenGLVertexArrayObject;
class MeshReader;
class GLTextureCube;
class Cube;
class BoxRenderable;
class GlyphRenderable :public Renderable, public QObject
{
	Q_OBJECT
public:
	GlyphRenderable(Cubemap* r);

	void init() override;
	//void resize(int width, int height) override;
	virtual void draw(float modelview[16], float projection[16]) override;
	void UpdateData() override;
	//virtual void cleanup() override;
	void SetCubemap(Cubemap* r) { cubemap = r; }
	//void GenCubesFromLine();

private:
	std::vector<GLTextureCube*> textures;
	void GenVertexBuffer(int nv, float* vertex, float* normal);
	void LoadShaders();
	unsigned int vbo_norm;
	unsigned int vbo_vert;
	int numElements = 0;
	MeshReader* glyphMesh;

	ShaderProgram *glProg;

	QOpenGLVertexArrayObject* m_vao;
	std::vector <Cube*> cubes;
	std::vector <BoxRenderable*> bboxes;

	Cubemap* cubemap;
	bool updated = false;

public slots:
	void SlotGenCubeAlongLine(float4* line, int nv);
	//void SlotGenCubeOnPlane(int planeIdx,);
	void SlotTest(){}

};
#endif //GLYPH_RENDERABLE_H
