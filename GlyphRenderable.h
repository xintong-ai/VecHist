#ifndef GLYPH_RENDERABLE_H
#define GLYPH_RENDERABLE_H

#include "Renderable.h"
#include <QObject>

class Cubemap;
class ShaderProgram;
class QOpenGLVertexArrayObject;
class GLSphere;
class GLTextureCube;
struct Cube;
class BoxRenderable;

class GlyphRenderable :public Renderable, public QObject
{
	Q_OBJECT
public:
	void init() override;
	void draw(float modelview[16], float projection[16]) override;
	void UpdateData() override;
	
	GlyphRenderable(Cubemap* r);
	void SetCubemap(Cubemap* r) { cubemap = r; }
	void SetVolumeDim(int x, int y, int z){ dataDim[0] = x; dataDim[1] = y; dataDim[2] = z; }
	int GetSliceDimIdx(){ return sliceDimIdx; }
	void SetHeightScale(int i) { heightScale = i; }
	GLTextureCube* GetCubeTexture(int i) { 
		if (textures.size() > i)
			return textures[i];
		else
			return nullptr;
	}
private:
	std::vector<GLTextureCube*> textures;
	std::vector <Cube*> cubes;
	void GenVertexBuffer(int nv, float* vertex);
	void LoadShaders();
	unsigned int vbo_vert;
	int sliceStart = 0;
	int numGlyphPerDim = 1;
	int sliceDimIdx = 0;// 0 is x, 1 is y, 2 is z
	int heightScale = 10;
	GLSphere* glyphMesh;
	ShaderProgram *glProg;
	ShaderProgram *glPickingProg;

	QOpenGLVertexArrayObject* m_vao;
	std::vector <BoxRenderable*> bboxes;

	Cubemap* cubemap;
	bool updated = false;
	int dataDim[3];
	bool cubesVisible = false;
protected:
	void mousePress(int x, int y, int modifier) override;

public slots:
	void SlotGenCubeAlongLine(float4* line, int nv);
	void SlotSliceNumChanged(int i);
	void SlotNumPartChanged(int i);
	void SlotHeightScaleChanged(int i);
	void SlotSetSliceOrie(int i);
	void SlotSetCubesVisible(bool visible);

signals:
	void SigChangeTex(GLTextureCube* v, Cube* c);
};
#endif //GLYPH_RENDERABLE_H
