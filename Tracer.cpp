#include "Tracer.h"
#include "DataMgr.h"

void Tracer::AllocOutImage() {
    if(h_output != NULL)
        delete [] h_output;

    h_output = new uint[winWidth * winHeight];
}

Tracer::~Tracer() {
    if(h_output != NULL)
        delete [] h_output;
}

void Tracer::resize(int width, int height) {
    winWidth = width;
    winHeight = height;
    //AllocOutImage();
}

void Tracer::draw(float modelview[16], float projection[16])
{
	memcpy(&matrix_mv.v[0].x, modelview, sizeof(float4) * 4);
	memcpy(&matrix_pj.v[0].x, projection, sizeof(float4) * 4);
}

//void Tracer::GetDataDim(int &nx, int &ny, int &nz) {
//    dataMgr->GetDataDim(nx, ny, nz);
//}
