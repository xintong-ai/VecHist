#include "DataManager.h"

class DataMgrVect:public DataManager
{
	float* data;
	int3* dataIdx;	//indexing the data with the cubemap
	int3* curlIdx;	//indexing the curl with the cubemap
	int initBlockSize;
	int blockDim[3];
	float3* data_x_first;
	//	int nCells;
	std::vector<std::vector<double> > vV;
	std::vector<std::vector<int> > vF;
	std::vector<float> vertexValue;
	std::vector<std::vector<double> > vVScaled;
	float* cubemap_data;// (new float[size * size * 6]);

	OSUFlow *osuflow;
	vector < vector<float4> > streamlines;
	vector < vector<float4> > streamlinesInCube;
	//9.5 is for nek
	//
	float entropyThreshold;// 6;// 10.2;	//592 cubes for plume
	Node *topNode;
	NodeBi *rootNode;
	int numBlocks;
	map<string, string> filenames;

	void SplitNode(Node* parent);
	void ComputeCubemapNode(Node *&nd);
	void ComputeCubemapNode(NodeBi *&nd);
	void GetDescendantNodes(vector<NodeBi*> &ret, NodeBi* nd);
	void LoadOSUFlow(const char* filename);
	void readBinaryTree(NodeBi *&p, ifstream &fin, vector<float3> starts, vector<float3> dims,
		vector<float> entropys, vector<float3> eig_vals, vector<float3> eig_vecs);
	void LoadParameters();


public:
	DataMgrVect();
	~DataMgrVect();

	void LoadVec(const char* filename);
	void LoadData();
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
	int3 GetVolumeDim();
	float3 GetQCubeCenter();
	void ResizeCube(int x, int y, int z);
	bool CubeInsideVolume(int x, int y, int z, int nx, int ny, int nz);

	int GetNumOfCells();
	void Segmentation();
	void LoadSegmentation();
	void SplitTopNode();
	void BuildOctree(Node *nd);
	vector<vector<float4>> GetStreamlines();
	vector<vector<float4>> GetStreamlinesInCube();
	void GenStreamInCube();
	float* GetVecData();
	float* GetVecDataXFirst();
	string GetFilename(string name);

	virtual vector<NodeBi*> GetAllNode();
	virtual NodeBi* getRootNode();
	virtual void UpdateCubeMap(float* cubemap);

};
