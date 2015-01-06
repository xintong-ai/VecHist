#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <vector>
#include <vector_types.h>
#include <vector_functions.h>

class DataManager
{
	float* data;
	int3* dataIdx;	//indexing the data with the cubemap
	int3* curlIdx;	//indexing the curl with the cubemap
	int dim[3];
	int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
	std::vector<std::vector<double> > vV;
	std::vector<std::vector<int> > vF;
	std::vector<float> vertexValue;
	std::vector<std::vector<double> > vVScaled;
	float* cubemap_data;// (new float[size * size * 6]);
	int cubemap_size;
public:
	void LoadVec(char* filename);
	std::vector<std::vector<double> > GetVertexPos();
	std::vector<std::vector<double> > GetScaledVertexPos();
	std::vector<std::vector<int> > GetFaceVertices();
	std::vector<float> GetVertexValue();
	std::vector<float3> GetBlock(int x, int y, int z, int nx, int ny, int nz);
	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap, int size);
	void IndexVolume(int size);
	void ComputeCurl();
	void QueryByBin(int f, int x, int y, unsigned char* result);
	void UpdateCubeMap(float* cubemap, int size);
	void GetVolumeSize(int &nx, int &ny, int&nz);
	void GetQCube(int &x, int &y, int &z, int &nx, int &ny, int &nz);
	void SetQCube(int x, int y, int z, int nx, int ny, int nz);
	int GetCubemapSize();
	int GetNumOfCells();
	~DataManager();
};

#endif //DATA_MANAGER_H