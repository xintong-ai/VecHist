#ifndef GLYPH_RENDERABLE_H
#define GLYPH_RENDERABLE_H

#include "Renderable.h"

class Cubemap;
class GLTextureCube;
class GlyphRenderable :public Renderable
{
public:
	GlyphRenderable(Cubemap* r);

	//void init() override;
	//void resize(int width, int height) override;
	virtual void draw(float modelview[16], float projection[16]) override;
	void UpdateData() override;
	//virtual void cleanup() override;
	void SetCubemap(Cubemap* r) { cubemap = r; }

private:
	std::vector<GLTextureCube*> textures;

	Cubemap* cubemap;
};
#endif //GLYPH_RENDERABLE_H
