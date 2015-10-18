#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <vector_types.h>
#include <vector_functions.h>
#include <vector>

struct Cube{
	int3 pos;
	int3 size;
	float* data = nullptr;
	int cubemap_size = 0;
	Cube(int x, int y, int z, int nx, int ny, int nz, int cubemap_size) {
		pos = make_int3(x, y, z);
		size = make_int3(nx, ny, nz);
		data = new float[cubemap_size * cubemap_size * 6];
	}
	Cube(int x, int y, int z, int nx, int ny, int nz){
		pos = make_int3(x, y, z);
		size = make_int3(nx, ny, nz);
	}
	~Cube(){
		if (nullptr != data) {
			delete[] data;
		}
	}
	int GetTotalSize() {
		return size.x * size.y * size.z;
	}
};

class VecReader;
class Cubemap{
	VecReader* vecReader;
	//float* data;
	int3* dataIdx;	//indexing the data with the cubemap
	float* cubemap_data;// (new float[size * size * 6]);
	int dim[3];
	int cubemap_size;
	int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
	bool CubeInsideVolumeX(int x, int nx);
	bool CubeInsideVolumeY(int x, int nx);
	bool CubeInsideVolumeZ(int x, int nx);
	void UpdateCubeMap(float* cubemap);
	void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz);
	void ComputeCurl();
	void QueryByBin(int f, int x, int y, unsigned char* result);

public:
	Cubemap(VecReader* r);
	~Cubemap();
	float3 GetQCubeCenter();
	void ResizeCube(int x, int y, int z);
	void IndexVolume(int size);
	int GetCubemapSize() { return cubemap_size; }
//	std::vector <Cube*> GetCubes() { return cubes; }
	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap, int& _cubemap_size);
//	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap);

};

#endif //CUBEMAP_H