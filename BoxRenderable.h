#ifndef BOX_RENDERABLE_H
#define BOX_RENDERABLE_H

#include "Renderable.h"

class VecReader;
class BoxRenderable:public Renderable
{
public:
	BoxRenderable(VecReader* r);
	BoxRenderable(float x, float y, float z, float nx, float ny, float nz);
	BoxRenderable(int3 _pos, int3 _dim);

	//void init() override;
	//void resize(int width, int height) override;
	virtual void draw(float modelview[16], float projection[16]) override;
	//virtual void cleanup() override;
	//void SetVecReader(VecReader* r) { vecReader = r; }

private:
	float3 pos;
	float3 dim;
	//VecReader* vecReader;
};
#endif //BOX_RENDERABLE_H