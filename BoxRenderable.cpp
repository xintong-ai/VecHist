#include "BoxRenderable.h"
#include "VecReader.h"
#include <QOpenGLWidget>

BoxRenderable::BoxRenderable(VecReader* r)
{
	SetVecReader(r);
}


void BoxRenderable::draw(float modelview[16], float projection[16])
{
	int3 dim = vecReader->GetVolumeDim();
	
//	int nx, ny, nz;

	int bx = dim.x - 1, by = dim.y - 1, bz = dim.z - 1;

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

}