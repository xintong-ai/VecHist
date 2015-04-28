#include "DataManager.h"



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
	float pos[3];
	float radius;
public:
	Halo(int haloid, float haloX, float haloY, float haloZ,
		float haloRadius, 
		float3 eig_vals,
		float3 eig_vec_0, float3 eig_vec_1, float3 eig_vec_2,
		float* halo_cubemap, int _cube_size) : AbstractNode(_cube_size)
	{
		pos[0] = haloX;
		pos[1] = haloY;
		pos[2] = haloZ;
		radius = haloRadius * 5;

		for (int i = 0; i < 3; i++)	{
			start[i] = pos[i] - radius;
			dim[i] = radius * 2;
		}

		radius = haloRadius;

		//eigval = EigenValue;

		//eigvec[0] = EigenVector1;
		//eigvec[1] = EigenVector2;
		//eigvec[2] = EigenVector3;

		//cubemap = halo_cubemap;
		cubemap = new float[cube_size * cube_size * 6];
		CubemapConvert(cubemap, halo_cubemap, cube_size);

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
	void LoadHalos();
	void LoadHalosBinary();
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
	MergeNode * readMergeTree(ifstream &fin);

	DataMgrCosm();
	~DataMgrCosm();

};
