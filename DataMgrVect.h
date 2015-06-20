#ifndef DATAMGRVECT
#define DATAMGRVECT
#include <fstream>

#include <vtkLookupTable.h>
#include "DataManager.h"

class NodeBi:public AbstractNode
{

	int level;	//hierarchy level
	//int3 *data;
	//vector<Node*> children;
	//only outgoing flux is recorded, so that we only draw the link once for the two neighboring nodes
	float flux[6];	//0: -x  1: +x   2: -y   3: +y   4: -z   5:+z

	//Widget::Node
public:
	//TODO: these two variable should be private
	NodeBi* left;	//Left child node
	NodeBi* right;	//Right child node
	NodeBi* original = nullptr; //Reference to the corresponding node in the original, regular entropy tree (if this is the master tree)
	NodeBi* master = nullptr;   //Reference to the corresponding node in the master tree (if this is the regular entropy tree)

	NodeBi(
		int _start0, int _start1, int _start2,
		int _dim0, int _dim1, int _dim2,
		float entropy,
		float3 eig_vals, float3 eig_vec_0, float3 eig_vec_1, float3 eig_vec_2, float * cubemap,
		int _cube_size, int _level) : AbstractNode(_cube_size)
	{
		//cubemap = _cubemap;
		//cube_size = _cube_size;
		//data = _data;

		start[0] = _start0;
		start[1] = _start1;
		start[2] = _start2;

		dim[0] = _dim0;
		dim[1] = _dim1;
		dim[2] = _dim2;

		this->entropy = entropy;

		//left = nullptr;
		//right = nullptr;
		//cubemap = nullptr;
		//cubemap = new float[_cube_size * _cube_size * 6];
		this->cubemap = cubemap;

		level = _level;

		left = nullptr;
		right = nullptr;

		glyph = new GLSuperquadric(eig_vals, eig_vec_0, eig_vec_1, eig_vec_2, 3, 0.5, 16);
		//for (int i = 0; i < 6; i++)	{
		//	neighbor[i] = nullptr;
		//	flux[6] = 0;
		//}

		isVisible = true;
		graphNode = nullptr;
	}

	//NodeBi(int *_start, int *_dim, int _cube_size, int _level) :
	//	NodeBi(_start[0], _start[1], _start[2],
	//	_dim[0], _dim[1], _dim[2],
	//	_cube_size, _level)
	//{}

	NodeBi* GetLeft(){ return left; }
	NodeBi* GetRight(){ return right; }
	//void GetStart(int* start);

	NodeBi():AbstractNode(){
		cubemap = nullptr;
	};

	~NodeBi()
	{
		//delete[] data;

		//TODO: delete this outside the destructor for CubeMaps
		//It cannot be deleted here, since we use shallow copies for copying the master tree to the regular entropy tree
		//if (cubemap != nullptr)
		//{
		//	delete[] cubemap;
			//cubemap = nullptr;
		//}
		
		//for (auto nd : children)	{
		//	delete[] nd;
		//}
		//delete[] left;
		//delete[] right;
	}
};

class DataMgrVect:public DataManager
{
	float* data;
	int3* dataIdx;	//indexing the data with the cubemap
	int3* curlIdx;	//indexing the curl with the cubemap
	int initBlockSize;
	int blockDim[3];
	float3* data_x_first;
	//	int nCells;
	double minEntropy = 0;  //Minimum entropy found in the entropy tree
	double maxEntropy = 0;  //Maximum entropy found in the entropy tree
	int minVolume = 0;		//Minimum computed volume found in the entropy tree
	int maxVolume = 0;		//Maximum computed volume found in the entropy tree
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
	//Node *topNode;
	NodeBi *rootNode;		//Root node to the entropy tree structure
	int numBlocks;

	vtkLookupTable * entropyColorTable = nullptr;  //Reference to the VTK color table for entropy (currently used in the entropy slider, graph structure widget, and tree map)
	vtkLookupTable * volumeColorTable = nullptr;  //Reference to the VTK color table for volume (currently used in the tree map widget)
	NodeBi * masterRootNode = nullptr;		//Root node of the master entropy tree.  This serves as a backup to restore the entropy tree when we make a subtree out of it.

	//void SplitNode(Node* parent);
	//void ComputeCubemapNode(Node *&nd);
	void ComputeCubemapNode(NodeBi *&nd);
	void GetDescendantNodes(vector<AbstractNode*> &ret, NodeBi* nd);
	void LoadOSUFlow(const char* filename);
	void readBinaryTree(NodeBi *&p, ifstream &fin, vector<float3> starts, vector<float3> dims,
		vector<float> entropys, vector<float3> eig_vals, vector<float3> eig_vecs, vector<float> cube_hists);
	void copyToMasterTree(NodeBi *&original, NodeBi *&master);
	void deleteEntropyTree(NodeBi * currentNode, int level);
	void copyMasterToEntropyTree(NodeBi *& regular, NodeBi *& master, int level);
	bool queryEntropyTreeByThreshold(double theshold, NodeBi * currentEntropyNode, NodeBi * currentMasterNode, int level);

public:
	DataMgrVect(AppSettings * appSettings);
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
	//void Segmentation();
	void LoadSegmentation();
	void BuildEntropyColorMap();
	void BuildVolumeColorMap();
	void getEntropyColor(double entropyValue, double color[3]);
	void getEntropyColorReversed(double entropyValue, double color[3]);
	void getVolumeColor(double volumeValue, double color[3]);
	void getVolumeColorReversed(double volumeValue, double color[3]);
	//void SplitTopNode();
	//void BuildOctree(Node *nd);
	vector<vector<float4>> GetStreamlines();
	vector<vector<float4>> GetStreamlinesInCube();
	void GenStreamInCube();
	float* GetVecData();
	float* GetVecDataXFirst();

	virtual vector<AbstractNode*> GetAllNode();
	virtual NodeBi* getRootNode();
	virtual void UpdateCubeMap(float* cubemap);
	void calculateExtremes();
	void calculateExtremes(NodeBi *p, bool useTreeLeavesForColorMap);
	float getMinEntropy() { return minEntropy; }
	float getMaxEntropy() { return maxEntropy; }
	int getMinVolume() { return minVolume; }
	int getMaxVolume() { return maxVolume; }

	void SetChildrenBelowEntropyToVisible(NodeBi * nd, double _maxEntropy);

	void PrintEntropies(NodeBi * nd, int level);

	void copyToMasterTree();
	bool queryEntropyTreeByThreshold(double threshold);
	void splitSuperQuadric(NodeBi * node);

};

#endif