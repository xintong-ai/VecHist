//These files must be included before including this file in a .cpp file:
//#include "DataManager.h"

#ifndef DATA_MGR_COSM
#define DATA_MGR_COSM
#include <list>


inline void CubemapConvert(float *out, float *in, int s)
{
	//for f = 0
	int s2 = s * s;
	for (int i = 0; i < s; i++)	{
		for (int j = 0; j < s; j++)	{
			out[0 * s2 + i * s + j] = in[4 * s2 + (s - 1 - i) * s + j];
			out[1 * s2 + i * s + j] = in[1 * s2 + i * s + j];
			out[2 * s2 + i * s + j] = in[5 * s2 + (s - 1 - i) * s + s - 1 - j];
			out[3 * s2 + i * s + j] = in[2 * s2 + (s - 1 - i) * s + j];
			out[4 * s2 + i * s + j] = in[3 * s2 + (s - 1 - j) * s + i];
			out[5 * s2 + i * s + j] = in[0 * s2 + j * s + i];
		}
	}
}


class Halo:public AbstractNode
{
public:
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
		pos[2] = haloZ;
		radius = haloRadius;

		for (int i = 0; i < 3; i++)	{
			start[i] = pos[i] - radius;
			dim[i] = radius * 2;
		}

		radius = haloRadius;

		eigval = eig_vals;

		eigvec[0] = eig_vec_0;
		eigvec[1] = eig_vec_1;
		eigvec[2] = eig_vec_2;

		//cubemap = halo_cubemap;
		cubemap = new float[cube_size * cube_size * 6];
		CubemapConvert(cubemap, halo_cubemap, cube_size);

		glyph = new GLSuperquadric(eig_vals, eig_vec_0, eig_vec_1, eig_vec_2, 3, 0.5, 16);
	}

	~Halo()
	{
		delete[] cubemap;
		delete glyph;
	}
};

//This class represents one node from the merger tree from the Dark Sky data
struct MergeNode
{
	vector<MergeNode *> children;  //Each node has an array of children
	int haloId = 0;	//The id of the halo corresponding to this node
	AbstractNode * haloRecord = nullptr;  //Reference to corresponding halo (if it is loaded and thus exists)
	int timeStepId = -1;				//Time step number (Stored as an integer -- thus 1.0 becomes 100, and 0.99 becomes 99)
	bool isVisible = true;  //Whether or not the entry in the merge tree is registered as visble (if it corresponds to a halo).  Not all nodes actually have halos loaded, so this must be stored here.
};

//This struct represents one merge tree as read from the Dark Sky merge tree data file
//TO DO: We may want to evaluate whether or not we want to keep this anymore given that we have discovered tree ids are in fact halo ids
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
	bool LoadHalosBinary(int timeStepId);
	bool LoadHalosBinary(string inputFileName);
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
	//QString buildJsonFromTreeList(list<MergeNode *> treeList);
	QString buildJsonFromTree(MergeNode * currentNode, int level);
	vector<MergeTree*> & getForest() { return forest; }
	MergeNode * readMergeTree(ifstream &fin, int treeId, int currentLevel);
	unordered_map<int, MergeNode *> * getMergeTreeTable() { return &mergeTreeTable; }
	void SetChildrenVisibility(MergeNode *nd, bool _isVisible);
	vector<MergeNode *> getNodesAtGivenTimestepFromForest(int desiredTimeStep);
	vector<MergeNode *> getNodesAtGivenTimestepFromTree(int desiredTimeStep, int currentTimeStep, MergeNode * mergeNode);
	

	DataMgrCosm();
	~DataMgrCosm();

};

#endif
