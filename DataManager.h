#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <vector>
#include <cuda/vector_types.h>
#include <cuda/vector_functions.h>
#include "OSUFlow.h"
#include "glSuperquadric.h"
#include "node.h"
#include "TreeMapWindow.h"
#include "AppSettings.h"
#include "windowsCpuTimer.h"
#include <unordered_map>

//1 means flow data
//2 means cosmology data
const int application = 1;

//A very small number used for floating point issue resolution
const double EPSILON = 1e-6;

namespace Widget
{
	class Node;
}

using namespace std;
//struct Node
//{
//	float *cubemap;
//	int cube_size;
//
//	int start[3];
//	int dim[3];
//	int level;	//hierarchy level
//	//int3 *data;
//	vector<Node*> children;
//	float entropy;
//	//only outgoing flux is recorded, so that we only draw the link once for the two neighboring nodes
//	float flux[6];	//0: -x  1: +x   2: -y   3: +y   4: -z   5:+z
//
//	Node* neighbor[6];
//
//	void init(
//		int _start0, int _start1, int _start2, 
//		int _dim0, int _dim1, int _dim2,
//		int _cube_size, int _level)
//	{
//		//cubemap = _cubemap;
//		cube_size = _cube_size;
//		//data = _data;
//
//		start[0] = _start0;
//		start[1] = _start1;
//		start[2] = _start2;
//
//		dim[0] = _dim0;
//		dim[1] = _dim1;
//		dim[2] = _dim2;
//		//left = nullptr;
//		//right = nullptr;
//		cubemap = nullptr;
//
//		level = _level;
//
//		for (int i = 0; i < 6; i++)	{
//			neighbor[i] = nullptr;
//			flux[6] = 0;
//		}
//	}
//
//
//
//	Node(){
//		cubemap = nullptr;
//	};
//
//	Node(
//		int _start0, int _start1, int _start2, 
//		int _dim0, int _dim1, int _dim2,
//		int _cube_size, int _level)
//	{
//		init(_start0, _start1, _start2, 
//		_dim0, _dim1, _dim2,
//		_cube_size, _level);
//	}
//
////	Node(int *_start, int *_dim, int _cube_size, int _level);
//	Node(int *_start, int *_dim, int _cube_size, int _level)
//	{
//		init(_start[0], _start[1], _start[2],
//		_dim[0], _dim[1], _dim[2],
//		_cube_size, _level);
//	}
//
//
//	~Node()
//	{
//		//delete[] data;
//		if (cubemap != nullptr)
//		{
//			delete[] cubemap;
//			//cubemap = nullptr;
//		}
//		for (auto nd : children)	{
//			delete[] nd;
//		}
//		//delete[] left;
//		//delete[] right;
//	}
//};
//
//

class AbstractNode
{
protected:
	int start[3];
	int dim[3];
	float *cubemap;
	int cube_size;
	float entropy;

	float3 eigval;
	float3 eigvec[3];

	Widget::Node * graphNode = nullptr;	//Reference to the node in the regular tree graph widget
	AbstractNode * haloNode = nullptr;	//Reference to the halo object (Dark Sky data)
	//TODO: It was intended that this store the reference to each TreeMapPlot object so that update() can be called on them to force repaints
	//That doesn't seem to be possible with some of the current #include circular dependence issues.  Thus for now, it is storing a reference to the entire
	//window and forcing a repaint on the entire thing.  We should try to clean up the #inlcude situation later.
	TreeMapWindow * treeMapWindow = nullptr;	//Reference to the tree map widget's window
	GLSuperquadric* glyph;
	bool isVisible = true;
	bool isSelected = false;
	bool isQueriedNull = false;					//Is the queried entropy tree null at this node (The intention is to eventually only have the master tree be THE tree and not have a second tree data structure)
	GLTextureCube *texCube = nullptr;		//OpenGL texture cube used in rendering
public:
	//TODO: Use capital letter for method names always or don't - but make it consistent
	bool GetVisible(){ return isVisible; }
	void SetVisible(bool b){ isVisible = b; }
	bool GetSelected() { return isSelected; }
	void SetSelected(bool isSelected) { this->isSelected = isSelected; }
	Widget::Node* GetGraphNode(){ return graphNode; }
	void SetGraphNode(Widget::Node* n) { graphNode = n; }
	AbstractNode * GetHaloNode() { return haloNode; }
	void setHaloNode(AbstractNode * haloNode) { this->haloNode = haloNode; }
	//virtual vector<AbstractNode*> GetAllNode() = 0;
	GLSuperquadric* GetGlyph(){ return glyph; }
	float* GetCubemap(){ return cubemap; }
	void setTreeMapWindow(TreeMapWindow * treeMapWindow) { this->treeMapWindow = treeMapWindow; }
	TreeMapWindow * getTreeMapWindow() { return treeMapWindow; }
	void setTexCube(GLTextureCube * texCube) { this->texCube = texCube; }
	GLTextureCube * getTexCube() { return texCube; }
	void SetQueriedNull(bool isQueriedNull) { this->isQueriedNull = isQueriedNull; }
	bool GetQueriedNull() { return isQueriedNull; }

	void GetDim(int* _dim)
	{
		for (int i = 0; i < 3; i++)
			_dim[i] = dim[i];
	}

	void GetStart(int* _start)
	{
		for (int i = 0; i < 3; i++)
			_start[i] = start[i];
	}

	float GetEntropy() { return entropy; }
	void SetEntropy(float entropy) { this->entropy = entropy; }
	int GetVolume() { return dim[0] * dim[1] * dim[2]; }

	AbstractNode(int _cube_size)
	{
		cube_size = _cube_size;
	}

	AbstractNode(){}

};

class DataManager
{
protected:
	int currentTimeStep = 100;	//Timestep currently loaded in scene (for cosmological data)
	int startTimeStep = 100;	//First in series of timesteps selectable for cosmological data (currently only for the JSON component)
	int cubemap_size;
	int start[3];
	int dim[3];
	int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
	map<string, string> valStrings;

	bool CubeInsideVolumeX(int x, int nx);
	bool CubeInsideVolumeY(int x, int nx);
	bool CubeInsideVolumeZ(int x, int nx);
	unordered_map<int, AbstractNode *> haloTable;	//Hash table to link halo ids to Halo struct records
	AppSettings * appSettings;						//Reference to the app settings object
	WindowsCpuTimer timer;							//Timing object (done through Windows API's for now)

public:
	virtual void LoadData() = 0;
	virtual vector<AbstractNode*> GetAllNode() = 0;
	virtual AbstractNode* getRootNode() = 0/*{ return rootNode; }*/;
	virtual vector<vector<float4>> GetStreamlines() = 0;
	virtual void GenStreamInCube() = 0;
	virtual void GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz) = 0;
	virtual void GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap) = 0;
	virtual void UpdateCubeMap(float* cubemap) = 0;

	int GetCubemapSize();
	void GetVolumeSize(int &nx, int &ny, int&nz);
	void GetQCube(int &x, int &y, int &z, int &nx, int &ny, int &nz);
	void SetQCube(int x, int y, int z, int nx, int ny, int nz);
	void MoveCube(int x, int y, int z);
	void LoadParameters();
	string GetStringVal(string name);
	unordered_map<int, AbstractNode *> * getHaloTable() { return &haloTable; }
	int getCurrentTimeStep() { return currentTimeStep; }
	void setCurrentTimeStep(int currentTimeStep) { this->currentTimeStep = currentTimeStep; }
	int getStartTimeStep() { return startTimeStep; }
	void setStartTimeStep(int startTimeStep) { this->startTimeStep = startTimeStep; }

	DataManager(AppSettings * appSettings);
	~DataManager();
};

#endif //DATA_MANAGER_H