#ifndef GLYPH_RENDERABLE_H
#define GLYPH_RENDERABLE_H

#include "Renderable.h"
#include <QObject>

class Cubemap;
//class GLTextureCube;
class ShaderProgram;
class QOpenGLVertexArrayObject;
class GLSphere;
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
	void SetVolumeDim(int x, int y, int z){ dataDim[0] = x; dataDim[1] = y; dataDim[2] = z; }
	//void GenCubesFromLine();

private:
	std::vector<GLTextureCube*> textures;
	void GenVertexBuffer(int nv, float* vertex);
	void LoadShaders();
	unsigned int vbo_norm;
	unsigned int vbo_vert;
	int numElements = 0;
	int sliceStart = 0;
	int numGlyphPerDim = 1;
	int sliceDimIdx = 0;// 0 is x, 1 is y, 2 is z
	//MeshReader* glyphMesh;
	GLSphere* glyphMesh;

	ShaderProgram *glProg;

	QOpenGLVertexArrayObject* m_vao;
	std::vector <Cube*> cubes;
	std::vector <BoxRenderable*> bboxes;

	Cubemap* cubemap;
	bool updated = false;
	int dataDim[3];
	bool cubesVisible = false;

public slots:
	void SlotGenCubeAlongLine(float4* line, int nv);
	void SlotSliceNumChanged(int i);
	void SlotNumPartChanged(int i);
	
	//void SlotGenCubeOnPlane(int planeIdx,);
	void SlotTest(){}

};
#endif //GLYPH_RENDERABLE_H
