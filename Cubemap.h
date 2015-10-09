#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <vector_types.h>

class Cubemap{
	float* data;
	int3* dataIdx;	//indexing the data with the cubemap
	float* cubemap_data;// (new float[size * size * 6]);
	int dim[3];
	int cubemap_size;
	int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
	bool CubeInsideVolumeX(int x, int nx);
	bool CubeInsideVolumeY(int x, int nx);
	bool CubeInsideVolumeZ(int x, int nx);
	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap);
	void UpdateCubeMap(float* cubemap);
	void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz);
	void ComputeCurl();
	void QueryByBin(int f, int x, int y, unsigned char* result);

public:
	Cubemap();
	~Cubemap();
	float3 GetQCubeCenter();
	void ResizeCube(int x, int y, int z);
	void IndexVolume(int size);

};

#endif //CUBEMAP_H