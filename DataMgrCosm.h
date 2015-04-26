#include "DataManager.h"

class Halo:public AbstractNode
{
	float pos[3];
	float radius;
public:
	Halo(float haloX, float haloY, float haloZ,
		float haloRadius, 
		float3 eig_vals,
		float3 eig_vec_0, float3 eig_vec_1, float3 eig_vec_2,
		float* halo_cubemap, int _cube_size) : AbstractNode(_cube_size)
	{
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

class DataMgrCosm:public DataManager
{
	vector<Halo*> halos;
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

	DataMgrCosm();
	~DataMgrCosm();

};