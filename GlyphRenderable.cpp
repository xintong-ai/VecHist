#include "GlyphRenderable.h"
#include "Cubemap.h"
#include <QOpenGLWidget>
#include "glbuffers.h"
#include "glSuperquadric.h"

GlyphRenderable::GlyphRenderable(Cubemap* r)
{
	SetCubemap(r);
}

void GlyphRenderable::UpdateData()
{
	textures.clear();
	int cubemap_size = cubemap->GetCubemapSize();
	std::vector<Cube*> cubes = cubemap->GetCubes();
	for (int i = 0; i < cubes.size(); i++) {
		GLTextureCube* tex = new GLTextureCube(cubemap_size);
		tex->load(cubes[i]->data, cubemap_size);
		textures.push_back(tex);
	}
	//GLSuperquadric* mesh = new GLSuperquadric()
}


void GlyphRenderable::draw(float modelview[16], float projection[16])
{
}