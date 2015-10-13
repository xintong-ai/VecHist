#include "VecReader.h"


//#include "DataManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include "dstat.h"
#include <memory>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector_functions.h>
#include <fstream>

#include <vector>



//inline void TruncateVolume(int3* in, int3* out,
//	int dim0, int dim1, int dim2,
//	int x, int y, int z,
//	int nx, int ny, int nz)
//{
//	int dim12 = dim1 * dim2;
//	for (int ix = 0; ix < nx; ix++)	{
//		for (int iy = 0; iy < ny; iy++)	{
//			for (int iz = 0; iz < nz; iz++)	{
//				out[ix * ny * nz + iy * nz + iz] =
//					in[(x + ix) * dim12 + (y + iy) * dim2 + z + iz];
//			}
//		}
//	}
//}
//
//void VecReader::SplitTopNode()
//{
//	//$$$
//	//Node left, right;
//	for (int i = 0; i < 3; i++)	{
//		blockDim[i] = ceil((float)dim[i] / initBlockSize);
//	}
//	int numBlocks = blockDim[0] * blockDim[1] * blockDim[2];
//	Node **nd = new Node*[numBlocks];
//	for (int i = 0; i < blockDim[0]; i++)	{
//		for (int j = 0; j < blockDim[1]; j++)	{
//			for (int k = 0; k < blockDim[2]; k++)	{
//				int start[3] = { i * initBlockSize, j * initBlockSize, k * initBlockSize };
//				int blockSize[3] = {
//					min(initBlockSize, dim[0] - i * initBlockSize),
//					min(initBlockSize, dim[1] - j * initBlockSize),
//					min(initBlockSize, dim[2] - k * initBlockSize) };
//				nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k] =
//					new Node(start, blockSize, cubemap_size, 1);
//			}
//		}
//	}
//	float3* idata = static_cast<float3*>((void *)data);
//	for (int i = 0; i < (blockDim[0] - 1); i++)	{
//		for (int j = 0; j < blockDim[1]; j++)	{
//			for (int k = 0; k < blockDim[2]; k++)	{
//				Node* thisNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
//				Node* nextNode = nd[(i + 1) * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
//				int xa = thisNode->start[0] + thisNode->dim[0] - 1;
//				int xb = xa + 2;
//				int ya = thisNode->start[1];
//				int yb = thisNode->start[1] + thisNode->dim[1];
//				int za = thisNode->start[2];
//				int zb = thisNode->start[2] + thisNode->dim[2];
//				float sum = 0;
//				for (int x = xa; x < xb; x++)	{
//					for (int y = ya; y < yb; y++)	{
//						for (int z = za; z < zb; z++)	{
//							sum += idata[x * dim[1] * dim[2] + y * dim[2] + z].x;
//						}
//					}
//				}
//				if (sum > 0)
//					thisNode->flux[1] = sum;
//				else
//					nextNode->flux[0] = -sum;
//				thisNode->neighbor[1] = nextNode;
//				nextNode->neighbor[0] = thisNode;
//			}
//		}
//	}
//
//	for (int i = 0; i < blockDim[0]; i++)	{
//		for (int j = 0; j < (blockDim[1] - 1); j++)	{
//			for (int k = 0; k < blockDim[2]; k++)	{
//				Node* thisNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
//				Node* nextNode = nd[i * blockDim[1] * blockDim[2] + (j + 1) * blockDim[2] + k];
//				int xa = thisNode->start[0];
//				int xb = thisNode->start[0] + thisNode->dim[0];
//				int ya = thisNode->start[1] + thisNode->dim[1] - 1;
//				int yb = ya + 2;
//				int za = thisNode->start[2];
//				int zb = thisNode->start[2] + thisNode->dim[2];
//				float sum = 0;
//				for (int x = xa; x < xb; x++)	{
//					for (int y = ya; y < yb; y++)	{
//						for (int z = za; z < zb; z++)	{
//							sum += idata[x * dim[1] * dim[2] + y * dim[2] + z].y;
//						}
//					}
//				}
//				if (sum > 0)
//					thisNode->flux[3] = sum;
//				else
//					nextNode->flux[2] = -sum;
//				thisNode->neighbor[3] = nextNode;
//				nextNode->neighbor[2] = thisNode;
//			}
//		}
//	}
//
//	for (int i = 0; i < blockDim[0]; i++)	{
//		for (int j = 0; j < blockDim[1]; j++)	{
//			for (int k = 0; k < (blockDim[2] - 1); k++)	{
//				Node* thisNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
//				Node* nextNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k + 1];
//				int xa = thisNode->start[0];
//				int xb = thisNode->start[0] + thisNode->dim[0];
//				int ya = thisNode->start[1];
//				int yb = thisNode->start[1] + thisNode->dim[1];
//				int za = thisNode->start[2] + thisNode->dim[2] - 1;
//				int zb = za + 2;
//				float sum = 0;
//				for (int x = xa; x < xb; x++)	{
//					for (int y = ya; y < yb; y++)	{
//						for (int z = za; z < zb; z++)	{
//							sum += idata[x * dim[1] * dim[2] + y * dim[2] + z].z;
//						}
//					}
//				}
//				if (sum > 0)
//					thisNode->flux[5] = sum;
//				else
//					nextNode->flux[4] = -sum;
//				thisNode->neighbor[5] = nextNode;
//				nextNode->neighbor[4] = thisNode;
//			}
//		}
//	}
//
//	for (int i = 0; i < numBlocks; i++)
//	{
//		ComputeCubemapNode(nd[i]);
//		topNode->children.push_back(nd[i]);;
//	}
//	delete[] nd;
//}
//
//void VecReader::ComputeCubemapNode(Node *&nd)
//{
//	int blockDimTotal = nd->dim[0] * nd->dim[1] * nd->dim[2];
//	std::unique_ptr<int3[]> datablock(new int3[blockDimTotal]);
//	GetBlock(datablock.get(), nd->start[0], nd->start[1], nd->start[2], nd->dim[0], nd->dim[1], nd->dim[2]);
//	nd->cubemap = new float[cubemap_size * cubemap_size * 6];
//	ComputeCubeMap(datablock.get(), blockDimTotal, nd->cubemap, cubemap_size);
//}

//void VecReader::ComputeCubemapNode(NodeBi *&nd)
//{
//
//	int dim[3];
//	int start[3];
//	nd->GetDim(dim);
//	nd->GetStart(start);
//
//	int blockDimTotal = dim[0] * dim[1] * dim[2];
//	std::unique_ptr<int3[]> datablock(new int3[blockDimTotal]);
//	GetBlock(datablock.get(), start[0], start[1], start[2], dim[0], dim[1], dim[2]);
//	ComputeCubeMap(datablock.get(), blockDimTotal, nd->GetCubemap(), cubemap_size);
//}

//void VecReader::BuildOctree(Node *nd)
//{
//	//for (auto nd : node->children)	{
//	float entropy = CubemapEntropy(nd->cubemap, nd->cube_size);
//	nd->entropy = entropy;
//	cout << entropy << ",";
//	if (entropy > entropyThreshold)	{
//		//Node *node0 = new Node(...);
//		//BuildOctree(node0);
//		/////....
//		int childDimL[3], childDimR[3];
//		childDimL[0] = nd->dim[0] / 2;
//		childDimL[1] = nd->dim[1] / 2;
//		childDimL[2] = nd->dim[2] / 2;
//
//		childDimR[0] = nd->dim[0] - childDimL[0];
//		childDimR[1] = nd->dim[1] - childDimL[1];
//		childDimR[2] = nd->dim[2] - childDimL[2];
//
//		Node *child;
//
//		child = new Node(
//			nd->start[0], nd->start[1], nd->start[2],
//			childDimL[0], childDimL[1], childDimL[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0] + childDimL[0], nd->start[1], nd->start[2],
//			childDimR[0], childDimL[1], childDimL[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0], nd->start[1] + childDimL[1], nd->start[2],
//			childDimL[0], childDimR[1], childDimL[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0] + childDimL[0], nd->start[1] + childDimL[1], nd->start[2],
//			childDimR[0], childDimR[1], childDimL[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0], nd->start[1], nd->start[2] + childDimL[2],
//			childDimL[0], childDimL[1], childDimR[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0] + childDimL[0], nd->start[1], nd->start[2] + childDimL[2],
//			childDimR[0], childDimL[1], childDimR[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0], nd->start[1] + childDimL[1], nd->start[2] + childDimL[2],
//			childDimL[0], childDimR[1], childDimR[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//
//		child = new Node(
//			nd->start[0] + childDimL[0], nd->start[1] + childDimL[1], nd->start[2] + childDimL[2],
//			childDimR[0], childDimR[1], childDimR[2],
//			cubemap_size, nd->level + 1);
//		ComputeCubemapNode(child);
//		nd->children.push_back(child);
//		BuildOctree(child);
//	}
//	else
//	{
//		numBlocks += 1;
//	}
//	//}
//}
//


bool VecReader::CubeInsideVolume(int x, int y, int z, int nx, int ny, int nz)
{
	return x >= 0 && y >= 0 && z >= 0 && (x + nx) <= dim[0] && (y + ny) <= dim[1] && (z + nz) <= dim[2];
}


//This method begins the data loading process for vector field data
void VecReader::LoadData()
{
	ofstream perfLog;
	perfLog.open("perflog.log", ios::out);
	if (!perfLog.is_open()) {
		cerr << "perflog.log did not open successfully!" << endl;
	}


	//Load the vector data file and time the processing
	//timer.reinit();
	//timer.startTimer();
	LoadVec("D:/data/plume/15plume3d421-504x504x2048.vec");// GetStringVal("vectorfield").c_str());
	//timer.stopTimer();
	//int totalTime = timer.getTimeElapsed();
	//perfLog << "-------------------------------------------------------" << endl;
	//perfLog << "Vector data loading time:::" << endl;
	//perfLog << totalTime << " milliseconds" << endl;
	//perfLog << "-------------------------------------------------------" << endl;
	//perfLog << endl;
	//cout << "-------------------------------------------------------" << endl;
	//cout << "Vector data loading time:::" << endl;
	//cout << totalTime << " milliseconds" << endl;
	//cout << "-------------------------------------------------------" << endl;
	//cout << endl;

	//Load the segmentation - includes loading the binary tree structure and calculating the cube map for the textures (very important computation)
	//Time it as well.
	//timer.reinit();
	//timer.startTimer();
	//LoadSegmentation();
	//timer.stopTimer();
	//totalTime = timer.getTimeElapsed();
	//perfLog << "-------------------------------------------------------" << endl;
	//perfLog << "LoadSegmentation data loading time (includes cube map computation):::" << endl;
	//perfLog << totalTime << " milliseconds" << endl;
	//perfLog << "-------------------------------------------------------" << endl;
	//perfLog << endl;
	//cout << "-------------------------------------------------------" << endl;
	//cout << "LoadSegmentation data loading time (includes cube map computation):::" << endl;
	//cout << totalTime << " milliseconds" << endl;
	//cout << "-------------------------------------------------------" << endl;
	//cout << endl;

	//perfLog.close();
	
	//Build the color maps used with the tree widgets
	//calculateExtremes();
	//BuildEntropyColorMap();
	//BuildVolumeColorMap();
}


void VecReader::LoadVec(const char* filename)
{
	using namespace std;
	FILE* file;
	file = fopen(filename, "rb");
	if (file == NULL)
	{
		cout << "file open error:" << filename << endl;
	}
	cout << filename << endl;
	fread(dim, sizeof(int), 3, file);

	cout << dim[0] << "," << dim[1] << "," << dim[2] << endl;
	int dimtotal = dim[0] * dim[1] * dim[2];

	data_x_first = new float3[dimtotal];
	data = new float[dimtotal * 3];
	float3* idata = static_cast<float3*>((void *)data);

	fread(data_x_first, sizeof(float3), dimtotal, file);
	for (int i = 0; i < dim[0]; i++)	{
		for (int j = 0; j < dim[1]; j++)	{
			for (int k = 0; k < dim[2]; k++)	{
				idata[i * dim[1] * dim[2] + j * dim[2] + k] =
					data_x_first[k * dim[0] * dim[1] + j * dim[0] + i];
			}
		}
	}
	//initBlockSize = min(min(dim[0], dim[1]), dim[2]) / 3;



	fclose(file);
	cout << "Data loaded" << endl;




	//int start[3] = { 0, 0, 0 };
	//topNode = new Node(start, dim, cubemap_size, 0);
	//ComputeCurl();

}








int VecReader::GetNumOfCells()
{
	return dim[0] * dim[1] * dim[2];
}



VecReader::VecReader(const char* filename)//(AppSettings * appSettings) : DataManager(appSettings)
{
	vecFileName = std::string(filename);
	LoadVec(filename);// "");// GetStringVal("vectorfield").c_str());

	//numBlocks = 0;
	//entropyThreshold = 9;
}

VecReader::~VecReader()
{
	delete[] data;
	delete[] data_x_first;
	//delete[] dataIdx;
	//delete[] curlIdx;
	//delete[] topNode;

	//deleteEntropyTree(masterRootNode, 0);
}

//std::vector<std::vector<double> > VecReader::GetVertexPos()
//{
//	return vV;
//}

//std::vector<std::vector<int> > VecReader::GetFaceVertices()
//{
//	return vF;
//}

//std::vector<float> VecReader::GetVertexValue()
//{
//	return vertexValue;
//}
//
//std::vector<std::vector<double> > VecReader::GetScaledVertexPos()
//{
//	return vVScaled;
//}

//void VecReader::Segmentation()
//{
//	//topNode = new Node(cubemap_data, cubemap_size, dataIdx, dim);
//	SplitTopNode();
//	//uncomment later
//	for (auto child : topNode->children)
//	{
//		BuildOctree(child);
//	}
//	cout << "number of cubic blocks: " << numBlocks << endl;
//}

inline bool readNextToken(std::istream &in, int &token, bool &isNumber)
{
	//char c;
	//isNumber = false;

	//if (in.eof()) return false;

	//// skipping the space
	//while ((c = in.get()) == ' ')
	//	if (in.eof()) return false;

	//if (c >= '0' && c > token)
	//	isNumber = true;
	in >> token;
	if (in.eof())
		isNumber = false;
	//cout << token;
	isNumber = (token != -1);
	return isNumber;
}

//void VecReader::readBinaryTree(NodeBi *&p, ifstream &fin, vector<float3> & starts, vector<float3> & dims,
//	vector<float> & entropys, vector<float3> & eig_vals, vector<float3> & eig_vecs) {
//	int token;
//	bool isNumber;
//	if (!readNextToken(fin, token, isNumber))
//		return;
//	if (isNumber) {
//		p = new NodeBi(
//			starts[token].x, starts[token].y, starts[token].z,
//			dims[token].x, dims[token].y, dims[token].z,
//			entropys[token], eig_vals[token],
//			eig_vecs[3 * token], eig_vecs[3 * token + 1], eig_vecs[3 * token + 2],
//			cubemap_size, 0);
//		//cout << "New entropy record: "  << entropys[token] << endl;
//		ComputeCubemapNode(p);
//		readBinaryTree(p->left, fin, starts, dims, entropys, eig_vals, eig_vecs);
//		readBinaryTree(p->right, fin, starts, dims, entropys, eig_vals, eig_vecs);
//	}
//}

//This method takes th0e current entropy tree and builds the master tree as an exact copy of it
//The master tree serves as a permanent backup of the original tree so that we can destroy it as we please and restore it later
//void VecReader::copyToMasterTree()
//{
//	copyToMasterTree(rootNode, masterRootNode);
//}

//This method recursively builds the bulk of the master tree described in the method above
//Parameter original - the current entropy tree node being copied
//Parameter master - the current master tree node being created as a copy
//void VecReader::copyToMasterTree(NodeBi *& original, NodeBi *& master) 
//{
//	master = new NodeBi();
//	(*master) = (*original); //Use the default C++ implementation of the assignment operator.  We want a shallow copy of all pointers.
//
//	master->left = nullptr;
//	master->right = nullptr;
//	master->master = nullptr;
//
//	//Link the master record to the original record
//	original->original = nullptr; //Should already be set, but let's be sure
//	master->original = original;  
//		
//	if (original->left != nullptr) {
//		copyToMasterTree(original->left, master->left);
//	}
//
//	if (original->right != nullptr) {
//		copyToMasterTree(original->right, master->right);
//	}
//	
//}
//
////This function carries out all of the steps required to query for a subtree with a given entropy threshold, modeled after the Python flow
////The long term intent is to phase out the entropy sub tree so that only a master tree is used with a boolean isQueriedNull field implicitly representing the subtree
//bool VecReader::queryEntropyTreeByThreshold(double threshold)
//{
//	deleteEntropyTree(masterRootNode, 0);
//	copyMasterToEntropyTree(rootNode, masterRootNode, 0);
//	return queryEntropyTreeByThreshold(threshold, rootNode, masterRootNode, 0);
//}
//
////This function deletes the entire entropy query subtree
////It currently is used before copying the master tree to the entropy query subtree (which will be phased out), but it is also used in the destructor
//void VecReader::deleteEntropyTree(NodeBi * currentNode, int level)
//{
//	//cout << "Current level: " << level << endl;
//	if (currentNode->left != nullptr) {
//		deleteEntropyTree(currentNode->left, level + 1);
//	}
//
//	if (currentNode->right != nullptr) {
//		deleteEntropyTree(currentNode->right, level + 1);
//	}
//
//	if (currentNode->original != nullptr) {
//		delete currentNode->original;
//	}
//
//	currentNode->original = nullptr;
//
//}
//
////This function copies the entropy tree to the query result suubtree
////The intent is to phase out this method because the technique leads to obscure bugs.  See the discussion in queryEntropyTreeByThreshold for more info
////Parameters: regular - current entropy query subtree node
////master - current master tree node
////level - current level in tree (0 based) - useful for debugging
//void VecReader::copyMasterToEntropyTree(NodeBi *& regular, NodeBi *& master, int level)
//{
//	//cout << "Current level: " << currentLevel << endl;
//	regular = new NodeBi();
//	(*regular) = (*master); //Use the default C++ implementation of the assignment operator.  We want a shallow copy of all pointers.
//	
//	regular->left = nullptr;
//	regular->right = nullptr;
//
//	//Link the master record to the entropy tree record
//	regular->original = nullptr; //Should already be set, but let's be sure
//	master->original = regular;
//	regular->master = master;
//
//	if (master->left != nullptr) {
//		copyMasterToEntropyTree(regular->left, master->left, level + 1);
//	}
//
//	if (master->right != nullptr) {
//		copyMasterToEntropyTree(regular->right, master->right, level + 1);
//	}
//
//}
//
////This function queries the entropy tree to find nodes that would have passed the entropy threshold test in the python flow
////It then builds a subtree to represent this.  The master tree is always maintained but referenced
////The intent, however, is to phase out the subtree and use only the master tree, marking the isQueriedNull flag in each node to represent the subtree implicitly
////We are having to mark data in the master tree anyways, so we might as well make the transition - the current method is very prone to difficult to debug pointer bugs when we make changes
////Parameters:
////threshold - the entropy query threshold (corresponding to the Python flow)
////currentEntropyNode - our current position in the subtree we are forming as a result of the query
////currentMaterNode - our current position in the master tree
////level - an 0 based integer reprenting how deep we are in the tree structure - useful for debugging sometimes
//bool VecReader::queryEntropyTreeByThreshold(double threshold, NodeBi * currentEntropyNode, NodeBi * currentMasterNode, int level)
//{
//	//cout << "Current level" << level << endl;
//	bool queryResultChanged = false; //This is false if the query subtree did not change from last time and true if it did.
//
//	//If we got to this node, it's obviously not null.  Mark it in the master tree
//	if (currentMasterNode->GetQueriedNull()) {
//		queryResultChanged = true;
//	}
//	currentMasterNode->SetQueriedNull(false);
//
//	if (currentEntropyNode->GetEntropy() <= threshold) {  //Opposite of inquality used in Python flow
//		//We have met the threshold and can stop recursing
//
//		//Once we set a node to null, we have no way to delete it again
//		//Thus we must do this now to avoid memory leaks that slowly build up as we query again and again and again...
//		if (currentEntropyNode->left != nullptr) {
//			currentMasterNode->left->original = nullptr;
//			delete currentEntropyNode->left;
//		}
//		if (currentEntropyNode->right != nullptr) {
//			currentMasterNode->right->original = nullptr;
//			delete currentEntropyNode->right;
//		}
//
//		//Because we have met the threshold, mark the child nodes null
//		currentEntropyNode->left = nullptr;
//		currentEntropyNode->right = nullptr;
//		if (currentMasterNode->left != nullptr) {
//			if (!currentMasterNode->left->GetQueriedNull()) {
//				queryResultChanged = true;
//			}
//
//			currentMasterNode->left->SetQueriedNull(true);
//		}
//		if (currentMasterNode->right != nullptr) {
//			if (!currentMasterNode->right->GetQueriedNull()) {
//				queryResultChanged = true;
//			}
//			currentMasterNode->right->SetQueriedNull(true);
//		}
//	}
//	else {
//		//Recurse to the next level if we did not meet the threshold
//		if (currentEntropyNode->left != nullptr) {
//			queryResultChanged = queryResultChanged | queryEntropyTreeByThreshold(threshold, currentEntropyNode->left, currentMasterNode->left, level + 1);  //Note: bitwise or is required to prevent short circuiting and not calling the function
//		}
//		if (currentEntropyNode->right != nullptr) {
//			queryResultChanged = queryResultChanged | queryEntropyTreeByThreshold(threshold, currentEntropyNode->right, currentMasterNode->right, level + 1);
//		}
//	}
//	
//	return queryResultChanged;
//
//}
//
//void VecReader::splitSuperQuadric(NodeBi * node)
//{
//	
//	if (node->left == nullptr && node->right == nullptr) { //Only a leaf node can be split
//		if (node->master != nullptr && (node->master->left != nullptr || node->master->right != nullptr)) { //There must be at least one child in the corresponding master tree node to do a split
//			if (node->master->left != nullptr) {
//				node->left = new NodeBi();
//				(*node->left) = (*node->master->left);
//				node->master->left->original = node->left;
//				node->left->master = node->master->left;
//				node->left->left = nullptr;
//				node->left->right = nullptr;
//			}
//			if (node->master->right != nullptr) {
//				node->right = new NodeBi();
//				(*node->right) = (*node->master->right);
//				node->master->right->original = node->right;
//				node->right->master = node->master->right;
//				node->right->left = nullptr;
//				node->right->right = nullptr;
//			}
//		}
//	}
//
//	//node->GetGraphNode()->update();
//	//node->
//
//
//}


inline std::vector<float3> ReadAttribute(const char* filename)
{
	std::vector<float3> ret;
	std::ifstream fin(filename, std::fstream::in);
	cout << filename << endl;
	while (true)
	{
		float v0, v1, v2;
		fin >> v0;
		fin >> v1;
		fin >> v2;
		if (fin.eof())
			break;
		ret.push_back(make_float3(v0, v1, v2));
	}
	return ret;
}

inline std::vector<float> ReadAttribute1D(const char* filename)
{
	std::vector<float> ret;
	std::ifstream fin(filename, std::fstream::in);
	while (true)
	{
		float v;
		fin >> v;
		if (fin.eof())
			break;
		ret.push_back(v);
	}
	return ret;
}

//void VecReader::LoadSegmentation()
//{
//	std::ifstream fin(valStrings["tree"], std::fstream::in);
//	std::vector<float3> starts = ReadAttribute(valStrings["starts"].c_str());
//	std::vector<float3> dims = ReadAttribute(valStrings["dims"].c_str());
//	std::vector<float> entropys = ReadAttribute1D(valStrings["entropy"].c_str());
//	std::vector<float3> eig_vals = ReadAttribute(valStrings["eigval"].c_str());
//	std::vector<float3> eig_vecs = ReadAttribute(valStrings["eigvec"].c_str());
//
//	int token;
//	bool isNumber;
//	readBinaryTree(rootNode, fin, starts, dims, entropys, eig_vals, eig_vecs);
//}

//This function builds the entropy vtk color map used for both the gradient in the slider widget and is used to determine the colors of graph nodes (tree structure widget and tree map)
//It corresponds to different values of entropy
//void VecReader::BuildEntropyColorMap()
//{	
//	entropyColorTable = vtkLookupTable::New();
//
//	entropyColorTable->SetHueRange(0.0, 0.55); //Was 0.0 to 0.66
//
//	entropyColorTable->SetNumberOfColors(256);
//	entropyColorTable->SetNanColor(0.1, 0.1, 0.1, 1.0);
//	entropyColorTable->SetTableRange(minEntropy, maxEntropy);
//	//colorTable->SetRampToLinear();
//
//	//colorTable->SetValueRange(MIN, MAX);
//
//	entropyColorTable->Build();
//}

//This function builds the volume vtk color map used in the tree map
//It corresponds to different values of volume
//void VecReader::BuildVolumeColorMap()
//{
//	volumeColorTable = vtkLookupTable::New();
//
//	volumeColorTable->SetHueRange(0.0, 0.55); //Was 0.0 to 0.66
//
//	volumeColorTable->SetNumberOfColors(256);
//	volumeColorTable->SetNanColor(0.1, 0.1, 0.1, 1.0);
//	volumeColorTable->SetTableRange(minVolume, maxVolume);
//	//colorTable->SetRampToLinear();
//
//	//colorTable->SetValueRange(MIN, MAX);
//
//	volumeColorTable->Build();
//}

//This function gets an entropy color based on the values in the vtk color map
//Notably, it flips the result so that blue is the lowest value and so that red is the largest value
//void VecReader::getEntropyColor(double entropyValue, double color[3])
//{
//	if (entropyColorTable == nullptr) {
//		color[0] = color[1] = color[2] = 0.0;
//		cerr << "getEntropyColor was called, but the color map was null" << endl;
//	}
//	else {
//		entropyValue = entropyValue - minEntropy;
//		entropyValue = maxEntropy - minEntropy - entropyValue;
//		entropyValue = entropyValue + minEntropy;
//		
//		entropyColorTable->GetColor(entropyValue, color);
//	}
//}

//This function gets an entropy color based on the values in the vtk color map
//Notably, it does not flip the result -- thus making it "reversed" from what we normally use;
//this is what the gradient uses.
//void VecReader::getEntropyColorReversed(double entropyValue, double color[3])
//{
//	if (entropyColorTable == nullptr) {
//		color[0] = color[1] = color[2] = 0.0;
//		cerr << "getEntropyColorReversed was called, but the color map was null" << endl;
//	}
//	else {
//		entropyColorTable->GetColor(entropyValue, color);
//	}
//}

//This function gets a volume color based on the values in the vtk color map
//Notably, it flips the result so that blue is the lowest value and so that red is the largest value
//void VecReader::getVolumeColor(double volumeValue, double color[3])
//{
//	if (volumeColorTable == nullptr) {
//		color[0] = color[1] = color[2] = 0.0;
//		cerr << "getVolumeColor was called, but the color map was null" << endl;
//	}
//	else {
//		volumeValue = volumeValue - minVolume;
//		volumeValue = maxVolume - minVolume - volumeValue;
//		volumeValue = volumeValue + minVolume;
//		
//		volumeColorTable->GetColor(volumeValue, color);
//	}
//
//}

//This function gets a volume color based on the values in the vtk color map
//Notably, it does not flip the result -- thus making it "reversed" from what we normally use;
//this is what the gradient uses.
//void VecReader::getVolumeColorReversed(double volumeValue, double color[3])
//{
//	if (volumeColorTable == nullptr) {
//		color[0] = color[1] = color[2] = 0.0;
//		cerr << "getVolumeColorReversed was called, but the color map was null" << endl;
//	}
//	else {
//		volumeColorTable->GetColor(volumeValue, color);
//	}
//
//}

//vector<AbstractNode*> VecReader::GetAllNode()
//{
//	vector<AbstractNode*> ret;
//	GetDescendantNodes(ret, rootNode);
//	return ret;
//}
//
////This function returns the root node from the entropy split tree
//NodeBi* VecReader::getRootNode()
//{ 
//	return rootNode; 
//}
//

//void VecReader::GetDescendantNodes(vector<AbstractNode*> &ret, NodeBi* nd)
//{
//	//vector<NodeBi*> children;
//	//children.push_back(nd->left);
//	//children.push_back(nd->right);
//	//for (auto child : children){
//	//	if (child->left != nullptr)
//	//		GetDescendantNodes(ret, child);
//	//	else
//	//		ret.push_back(child);
//	//}
//	if (nd->GetLeft() == nullptr && nd->GetRight() == nullptr)
//		ret.push_back(nd);
//	else
//	{
//		GetDescendantNodes(ret, nd->GetLeft());
//		GetDescendantNodes(ret, nd->GetRight());
//	}
//}
//


//
//float* VecReader::GetVecData()
//{
//	return data;
//}
//
//float* VecReader::GetVecDataXFirst()
//{
//	return (float*)data_x_first;
//}

//
//
//VecReader::VecReader()
//{
//}
//
//VecReader::~VecReader()
//{
//}


//This function calculates the min and max entropy/volume values, storing them in class members minEntropy and maxEntropy / minVolume and maxVolume respectively
//void VecReader::calculateExtremes()
//{
//	string useTreeLeavesForColorMapStr = GetStringVal("useTreeLeavesForColorMap");
//	bool useTreeLeavesForColorMap = false;
//	if (useTreeLeavesForColorMapStr == "0") {
//		useTreeLeavesForColorMap = false;
//	}
//	else {
//		useTreeLeavesForColorMap = true;
//	}
//
//	NodeBi *rootNode = getRootNode();
//
//	//Travel to a leaf node and use this for the starting value for the min/max data values (until we find the true min/max)
//	//Starting with a leaf node is required if useTreeLeavesForColorMap is true, but it also works if useTreeLeavesForColorMap is false.
//	NodeBi * currentNode = nullptr;
//	for (currentNode = rootNode; currentNode != nullptr && currentNode->left != nullptr; currentNode = currentNode->left);
//	minEntropy = maxEntropy = currentNode->GetEntropy();
//	minVolume = maxVolume = currentNode->GetVolume();
//	
//	calculateExtremes(rootNode, useTreeLeavesForColorMap);
//
//	//Prevent weird floating point comparison issues in entropy threshold queries
//	minEntropy -= EPSILON;
//	maxEntropy += EPSILON;
//}

//This function recursively finds the min and max entropy/volume values, storing them in class members minEntropy and maxEntropy / minVolume and maxVolume respectively
//It is called by calculateEntropyExtremes() above
//Input parameters:
//p - the entropy tree structure
//useTreeLeavesForColorMap - true if only tree laves should be used for min/max entropy values.  False if any node is assessed.  Has a big impact on colormap.
//void VecReader::calculateExtremes(NodeBi *p, bool useTreeLeavesForColorMap)
//{
//	float currentEntropy = p->GetEntropy();
//	int currentVolume = p->GetVolume();
//
//	if (!useTreeLeavesForColorMap || (p->left == nullptr && p->right == nullptr)) {
//		if (currentEntropy < minEntropy) {
//			minEntropy = currentEntropy;
//		}
//
//		if (currentEntropy > maxEntropy) {
//			maxEntropy = currentEntropy;
//		}
//
//		if (currentVolume < minVolume) {
//			minVolume = currentVolume;
//		}
//
//		if (currentVolume > maxVolume) {
//			maxVolume = currentVolume;
//		}
//		
//	}
//
//	if (p->left != nullptr) {
//		calculateExtremes(p->left, useTreeLeavesForColorMap);
//	}
//	if (p->right != nullptr) {
//		calculateExtremes(p->right, useTreeLeavesForColorMap);
//	}
//
//}
//
////This method queries for all nodes at or below a given maximum entropy value.  Nodes at or below the maximum are set to visible.
////Nodes above the maximum are set to invisible.
//void VecReader::SetChildrenBelowEntropyToVisible(NodeBi * nd, double _maxEntropy)
//{
//	if (nd != nullptr) {
//		//Store the visibility in the merge tree node (Some halos may not be loaded)
//		double myEntropy = nd->GetEntropy();
//		nd->SetVisible(nd->GetEntropy() <= _maxEntropy);
//		
//		if (nd->GetGraphNode() != nullptr) {
//			nd->GetGraphNode()->update();
//			
//		}
//		
//		SetChildrenBelowEntropyToVisible(nd->left, _maxEntropy);
//		
//		SetChildrenBelowEntropyToVisible(nd->right, _maxEntropy);
//		
//	}
//
//}
//
//Utility debugging method to print all entropies up to a certain level (currently up to level 3)
//Parameters:
//    nd -> the current NodeBI record
//    level -> the current level
//void VecReader::PrintEntropies(NodeBi * nd, int level)
//{
//	if (nd != nullptr && level <= 3) {
//		//Store the visibility in the merge tree node (Some halos may not be loaded)
//		cout << "Entropy: " << nd->GetEntropy();
//
//		PrintEntropies(nd->left, level + 1);
//
//		PrintEntropies(nd->right, level + 1);
//
//	}
//
//}
