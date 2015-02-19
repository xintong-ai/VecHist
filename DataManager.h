#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <vector>
#include <vector_types.h>
#include <vector_functions.h>
#include "OSUFlow.h"

using namespace std;
struct Node
{
	float *cubemap;
	int cube_size;

	int start[3];
	int dim[3];
	int level;	//hierarchy level
	//int3 *data;
	vector<Node*> children;
	float entropy;
	//only outgoing flux is recorded, so that we only draw the link once for the two neighboring nodes
	float flux[6];	//0: -x  1: +x   2: -y   3: +y   4: -z   5:+z

	Node* neighbor[6];

	Node(
		int _start0, int _start1, int _start2, 
		int _dim0, int _dim1, int _dim2,
		int _cube_size, int _level)
	{
		//cubemap = _cubemap;
		cube_size = _cube_size;
		//data = _data;

		start[0] = _start0;
		start[1] = _start1;
		start[2] = _start2;

		dim[0] = _dim0;
		dim[1] = _dim1;
		dim[2] = _dim2;
		//left = nullptr;
		//right = nullptr;
		cubemap = nullptr;

		level = _level;

		for (int i = 0; i < 6; i++)	{
			neighbor[i] = nullptr;
			flux[6] = 0;
		}
	}

	Node(int *_start, int *_dim, int _cube_size, int _level):
		Node(_start[0], _start[1], _start[2],
		_dim[0], _dim[1], _dim[2],
		_cube_size, _level)
	{}


	Node(){
		cubemap = nullptr;
	};

	~Node()
	{
		//delete[] data;
		if (cubemap != nullptr)
		{
			delete[] cubemap;
			//cubemap = nullptr;
		}
		for (auto nd : children)	{
			delete[] nd;
		}
		//delete[] left;
		//delete[] right;
	}
};

class DataManager
{
	float* data;
	int3* dataIdx;	//indexing the data with the cubemap
	int3* curlIdx;	//indexing the curl with the cubemap
	int dim[3];
	int initBlockSize;
	int blockDim[3];
	float3* data_x_first;
	//	int nCells;
	int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
	std::vector<std::vector<double> > vV;
	std::vector<std::vector<int> > vF;
	std::vector<float> vertexValue;
	std::vector<std::vector<double> > vVScaled;
	float* cubemap_data;// (new float[size * size * 6]);
	int cubemap_size;

	OSUFlow *osuflow = new OSUFlow();
	vector < vector<float4> > streamlines;
	vector < vector<float4> > streamlinesInCube;
	//9.5 is for nek
	//
	const float entropyThreshold = 9;// 6;// 10.2;	//592 cubes for plume
	Node *topNode;
	int numBlocks;
	void SplitNode(Node* parent);
	void ComputeCubemapNode(Node *&nd);
	void GetDescendantNodes(vector<Node*> &ret, Node* nd);
	void LoadOSUFlow(char* filename);


public:
	void LoadVec(char* filename);
	std::vector<std::vector<double> > GetVertexPos();
	std::vector<std::vector<double> > GetScaledVertexPos();
	std::vector<std::vector<int> > GetFaceVertices();
	std::vector<float> GetVertexValue();
	void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz);
	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap);
	void IndexVolume(int size);
	//void ComputeFlux();	//compute flux for three directions of each face
	void ComputeCurl();
	void QueryByBin(int f, int x, int y, unsigned char* result);
	void UpdateCubeMap(float* cubemap);
	void GetVolumeSize(int &nx, int &ny, int&nz);
	int3 GetVolumeDim();
	void GetQCube(int &x, int &y, int &z, int &nx, int &ny, int &nz);
	float3 GetQCubeCenter();
	void SetQCube(int x, int y, int z, int nx, int ny, int nz);
	void MoveCube(int x, int y, int z);
	void ResizeCube(int x, int y, int z);
	bool CubeInsideVolume(int x, int y, int z, int nx, int ny, int nz);
	bool CubeInsideVolumeX(int x, int nx);
	bool CubeInsideVolumeY(int x, int nx);
	bool CubeInsideVolumeZ(int x, int nx);
	int GetCubemapSize();
	int GetNumOfCells();
	void Segmentation();
	void SplitTopNode();
	void BuildOctree(Node *nd);
	vector<Node*> GetAllNode();
	vector<vector<float4>> GetStreamlines();
	vector<vector<float4>> GetStreamlinesInCube();
	void GenStreamInCube();
	float* GetVecData();
	float* GetVecDataXFirst();
	DataManager();
	~DataManager();
};

#endif //DATA_MANAGER_H