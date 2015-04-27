#ifndef DATA_MGR_COSM
#define DATA_MGR_COSM
#include "DataManager.h"

class Halo:public AbstractNode
{
	int id;
	float pos[3];
	float radius;
public:
	Halo(int haloId, float haloX, float haloY, float haloZ,
		float haloRadius, 
		float3 eig_vals,
		float3 eig_vec_0, float3 eig_vec_1, float3 eig_vec_2,
		float* halo_cubemap, int _cube_size) : AbstractNode(_cube_size)
	{
		id = haloId;
		pos[0] = haloX;
		pos[1] = haloY;
		pos[2] = haloX;
		radius = haloRadius * 10;

		for (int i = 0; i < 3; i++)	{
			start[i] = pos[i] - radius;
			dim[i] = radius * 2;
		}

		radius = haloRadius;

		//eigval = EigenValue;

		//eigvec[0] = EigenVector1;
		//eigvec[1] = EigenVector2;
		//eigvec[2] = EigenVector3;

		cubemap = halo_cubemap;
		glyph = new GLSuperquadric(eig_vals, eig_vec_0, eig_vec_1, eig_vec_2, 3, 0.5, 16);
	}

	~Halo()
	{
		delete[] cubemap;
	}
};

//This class represents one node from the merger tree from the Dark Sky data
struct MergeNode
{
	vector<MergeNode *> children;  //Each node has an array of children
	int haloId = 0;	//The id of the halo corresponding to this node
	AbstractNode * haloRecord = nullptr;
};

struct MergeTree
{
	MergeNode * root;
	int treeId;

	MergeTree()
	{
		root = nullptr;
		treeId = 0;
	}
};

class DataMgrCosm:public DataManager
{
	vector<Halo*> halos;
	vector<MergeTree *> forest;  //A forest of merge trees from the Dark Sky data
	unordered_map<int, MergeNode *> mergeTreeTable;  //Hash table to link halo ids to Halo struct records
	void LoadHalos();
public:
	virtual void LoadData();
	virtual vector<AbstractNode*> GetAllNode()
	{
		vector<AbstractNode*> ret;
		for (auto ha : halos)	{
			ret.push_back(ha);
		}
		return ret;
	};
	virtual AbstractNode* getRootNode(){ return (AbstractNode*)(halos.front()); }
	virtual vector<vector<float4>> GetStreamlines(){
		vector < vector<float4> > streamlines;
		return streamlines;
	};
	virtual void GenStreamInCube(){};
	virtual void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz){};
	virtual void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap){};
	virtual void UpdateCubeMap(float* cubemap){};
	void LoadMergeTree();
	QString getMergeTreeJSon(int treeId);
	QString buildJsonFromTree(MergeNode * currentNode, int level);
	vector<MergeTree*> & getForest() { return forest; }
	MergeNode * readMergeTree(ifstream &fin, int treeId);
	unordered_map<int, MergeNode *> & getMergeTreeTable() { return mergeTreeTable; }
	void SetChildrenVisibility(MergeNode *nd, bool _isVisible);

	DataMgrCosm();
	~DataMgrCosm();

};

#endif