#ifndef GLYPH_RENDERABLE_H
#define GLYPH_RENDERABLE_H

#include "Renderable.h"

class Cubemap;
//class GLTextureCube;
class ShaderProgram;
class QOpenGLVertexArrayObject;
class MeshReader;
class GLTextureCube;
class GlyphRenderable :public Renderable
{
public:
	GlyphRenderable(Cubemap* r);

	void init() override;
	//void resize(int width, int height) override;
	virtual void draw(float modelview[16], float projection[16]) override;
	void UpdateData() override;
	//virtual void cleanup() override;
	void SetCubemap(Cubemap* r) { cubemap = r; }

private:
	std::vector<GLTextureCube*> textures;
	void GenVertexBuffer(int nv, float* vertex, float* normal);
	void LoadShaders();
	unsigned int vbo_norm;
	unsigned int vbo_vert;
	int numElements = 0;
	MeshReader* m;

	ShaderProgram *glProg;

	QOpenGLVertexArrayObject* m_vao;


	Cubemap* cubemap;
	bool updated = false;
};
#endif //GLYPH_RENDERABLE_H
