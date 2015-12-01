#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <vector_types.h>
#include <vector_functions.h>
#include <vector>

struct Cube{
	int3 pos;
	int3 size;
	float* data = nullptr;
	int cubemap_size;
	int phase;
	float mag;
	//Cube(int x, int y, int z, int nx, int ny, int nz, int cubemap_size) {
	//	pos = make_int3(x, y, z);
	//	size = make_int3(nx, ny, nz);
	//	data = new float[cubemap_size * cubemap_size * 6];
	//}
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
	VecReader* vecReader = nullptr;
	int3* dataIdx = nullptr;	//indexing the data with the cubemap
	int dim [3];
	float vmin[3];
	float vmax[3];
	int cubemap_size = 0;
	void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz);
	void QueryByBin(int f, int x, int y, unsigned char* result);
	int mergeStep = 2;	//every step x step x step cube becomes internal unit.
	int innerDim[3];
	unsigned short* innerData = nullptr;// cubemap for each inner block
	float* avgMag = nullptr;
	void GetBlockXYZ(float3* out, float3* in, int x, int y, int z, int nx, int ny, int nz);
	void CountIndex(unsigned short* out, float3* in, int n);
	std::vector<float> solAng;//
	std::vector<Cube*> innerCubes;
	int mode = 0; //0: vector,  1: hist
public:
	Cubemap(VecReader* r);
	Cubemap(const char* filename);
	~Cubemap();
	void IndexVolume();
	void IndexVolumeByHist();
	int GetCubemapSize() { return cubemap_size; }
	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap, int& _cubemap_size);
	void GenCubeMapOptimized(int x, int y, int z, int nx, int ny, int nz, float* &cubemap, int& _cubemap_size, float &mag);
	int3 GetInnerDim(){ return make_int3(innerDim[0], innerDim[1], innerDim[2]); }
	int GetInnerDim(int i){ return innerDim[i]; }
	void LoadHist(const char* filename);
	int GetMode(){ return mode; }
	//std::vector<Cube*> GetCubes(int x, int y, int z, int nx, int ny, int nz);
	void GetCubes(int x, int y, int z, int nx, int ny, int nz, std::vector<Cube*> &ret);

};

#endif //CUBEMAP_H