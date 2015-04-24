#include "DataManager.h"

class DataMgrCosm:public DataManager
{
	
public:
	virtual void LoadData(){};
	virtual vector<NodeBi*> GetAllNode()
	{
		vector<NodeBi*> ret;
		return ret;
	};
	virtual NodeBi* getRootNode(){ NodeBi *rootNode;  return rootNode; };
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
