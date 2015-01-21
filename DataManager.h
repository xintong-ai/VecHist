#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <vector>
#include <vector_types.h>
#include <vector_functions.h>

using namespace std;
struct Node
{
	float *cubemap;
	int cube_size;

	int start[3];
	int dim[3];
	//int3 *data;
	vector<Node*> children;
	float entropy;
	Node(int *_start, int *_dim, int _cube_size)
	{
		//cubemap = _cubemap;
		cube_size = _cube_size;
		//data = _data;

		start[0] = _start[0];
		start[1] = _start[1];
		start[2] = _start[2];

		dim[0] = _dim[0];
		dim[1] = _dim[1];
		dim[2] = _dim[2];
		//left = nullptr;
		//right = nullptr;
		cubemap = nullptr;
	};

	Node(
		int _start0, int _start1, int _start2, 
		int _dim0, int _dim1, int _dim2,
		int _cube_size)
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
	};

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
	//	int nCells;
	int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
	std::vector<std::vector<double> > vV;
	std::vector<std::vector<int> > vF;
	std::vector<float> vertexValue;
	std::vector<std::vector<double> > vVScaled;
	float* cubemap_data;// (new float[size * size * 6]);
	int cubemap_size;
	const float entropyThreshold = 9.5;// 6;// 10.2;	//592 cubes for plume
	Node *topNode;
	int numBlocks;
	void SplitNode(Node* parent);
	void ComputeCubemapNode(Node *&nd);
	void GetDescendantNodes(vector<Node*> &ret, Node* nd);

public:
	void LoadVec(char* filename);
	std::vector<std::vector<double> > GetVertexPos();
	std::vector<std::vector<double> > GetScaledVertexPos();
	std::vector<std::vector<int> > GetFaceVertices();
	std::vector<float> GetVertexValue();
	void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz);
	void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap);
	void IndexVolume(int size);
	void ComputeCurl();
	void QueryByBin(int f, int x, int y, unsigned char* result);
	void UpdateCubeMap(float* cubemap);
	void GetVolumeSize(int &nx, int &ny, int&nz);
	void GetQCube(int &x, int &y, int &z, int &nx, int &ny, int &nz);
	void SetQCube(int x, int y, int z, int nx, int ny, int nz);
	int GetCubemapSize();
	int GetNumOfCells();
	void Segmentation();
	void SplitTopNode();
	void BuildOctree(Node *nd);
	vector<Node*> GetAllNode();
	DataManager();
	~DataManager();
};

#endif //DATA_MANAGER_H