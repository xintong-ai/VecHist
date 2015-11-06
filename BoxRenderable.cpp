#include "BoxRenderable.h"
#include "VecReader.h"
#include <QOpenGLWidget>

BoxRenderable::BoxRenderable(int3 d)
{
	//	SetVecReader(r);
	dim.x = d.x;
	dim.y = d.y;
	dim.z = d.z;
	pos = make_float3(0, 0, 0);
}


BoxRenderable::BoxRenderable(VecReader* r)
{
//	SetVecReader(r);
	int3 volDim = r->GetVolumeDim();
	dim = make_float3(volDim.x, volDim.y, volDim.z);
	pos = make_float3(0, 0, 0);
}

BoxRenderable::BoxRenderable(float x, float y, float z, float nx, float ny, float nz)
{
	pos = make_float3(x, y, z);
	dim = make_float3(nx, ny, nz);
}

BoxRenderable::BoxRenderable(int3 _pos, int3 _dim)
{
	pos = make_float3(_pos.x, _pos.y, _pos.z);
	dim = make_float3(_dim.x, _dim.y, _dim.z);
}



void BoxRenderable::draw(float modelview[16], float projection[16])
{
	
//	int nx, ny, nz;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	float bx = dim.x - 1, by = dim.y - 1, bz = dim.z - 1;

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(bx, 0, 0);

	glVertex3f(bx, 0, 0);
	glVertex3f(bx, by, 0);

	glVertex3f(bx, by, 0);
	glVertex3f(0, by, 0);

	glVertex3f(0, by, 0);
	glVertex3f(0, 0, 0);

	//////
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, bz);

	glVertex3f(bx, 0, 0);
	glVertex3f(bx, 0, bz);

	glVertex3f(bx, by, 0);
	glVertex3f(bx, by, bz);

	glVertex3f(0, by, 0);
	glVertex3f(0, by, bz);

	//////
	glVertex3f(0, 0, bz);
	glVertex3f(bx, 0, bz);

	glVertex3f(bx, 0, bz);
	glVertex3f(bx, by, bz);

	glVertex3f(bx, by, bz);
	glVertex3f(0, by, bz);

	glVertex3f(0, by, bz);
	glVertex3f(0, 0, bz);

	glEnd();
	glPopMatrix();

}