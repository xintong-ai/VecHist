#include "DataManager.h"
#include <stdio.h>
#include <iostream>
//#include "dstat.h"
#include <memory>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector_functions.h>

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

std::vector<float> ComputePatchSolidAngle(int size)
{
	float delta = 2.0 / size;
	std::vector<float> solAng(size * size);
	float a1, a2, sa;
	float b1, b2, sb;
	float c1, c2, sc;
	float d1, d2, sd;
	for (int i = 0; i < size; i++)	{
		for (int j = 0; j < size; j++)	{
			a1 = 2 - 2 * i * delta;
			a2 = 2 - 2 * j * delta;
			sa = 4 * asin(sin(atan(a1 * 0.5)) * sin(atan(a2 * 0.5)));

			b1 = 2 - 2 * i * delta - 2 * delta;
			b2 = 2 - 2 * j * delta;
			sb = 4 * asin(sin(atan(b1 * 0.5)) * sin(atan(b2 * 0.5)));

			c1 = 2 - 2 * i * delta;
			c2 = 2 - 2 * j * delta - 2 * delta;
			sc = 4 * asin(sin(atan(c1 * 0.5)) * sin(atan(c2 * 0.5)));

			d1 = 2 - 2 * i * delta - 2 * delta;
			d2 = 2 - 2 * j * delta - 2 * delta;
			sd = 4 * asin(sin(atan(d1 * 0.5)) * sin(atan(d2 * 0.5)));

			solAng[i * size + j] = (sa - sb - sc + sd) * 0.25;
		}
	}
	return solAng;

}

inline int3 XYZ2Idx(float3 d, int size)
{
	// The odd value is the (f, x, y) = (0, 0, 0)
	int f = -1;
	float x = -1, y = -1;
	if (d.x >= abs(d.y) && d.x > abs(d.z))
	{
		f = 0;
		x = (-d.z / abs(d.x) + 1) * 0.5;
		y = (-d.y / abs(d.x) + 1) * 0.5;
	}
	else if (d.x < -abs(d.y) && d.x <= -abs(d.z))
	{
		f = 1;
		x = (d.z / abs(d.x) + 1) * 0.5;
		y = (-d.y / abs(d.x) + 1) * 0.5;
	}
	else if (d.y >= abs(d.z) && d.y > abs(d.x))
	{
		f = 2;
		x = (d.x / abs(d.y) + 1) * 0.5;
		y = (d.z / abs(d.y) + 1) * 0.5;
	}
	else if (d.y < -abs(d.z) && d.y <= -abs(d.x))
	{
		f = 3;
		x = (d.x / abs(d.y) + 1) * 0.5;
		y = (-d.z / abs(d.y) + 1) * 0.5;
	}
	else if (d.z >= abs(d.x) && d.z > abs(d.y))
	{
		f = 4;
		x = (d.x / abs(d.z) + 1) * 0.5;
		y = (-d.y / abs(d.z) + 1) * 0.5;
	}
	else if (d.z < -abs(d.x) && d.z <= -abs(d.y))
	{
		f = 5;
		x = (-d.x / abs(d.z) + 1) * 0.5;
		y = (-d.y / abs(d.z) + 1) * 0.5;
	}
	//else
	//{
	//	std::cout << "(" << d.x << "," << d.y << "," << d.z << std::endl;
	////	continue;
	//}
	//if (x < 0 || y < 0 || f < 0 || x > 1 || y > 1 || f >= 6)
	//{
	//	std::cout << "a (" << d.x << "," << d.y << "," << d.z << std::endl;
	//	std::cout << "f (" << f << "," << x << "," << y << std::endl;
	//}

	return make_int3(f, x * size, y * size);

}

inline void ComputeCubeMap(int3* data, int nCells, float* cubemap, const int size)
{
	using namespace std;
	vector<int> binCnt(size * size * 6, 0);// (new int(size * size * 6));
	//int *binCnt = new int[size * size * 6];
	const int size2 = size * size;
	const int size3 = size * size * 6;

	float x, y;
	int binx, biny;
	int f = -1;

	//(s,t,r) selects one of the cube map face's 2D mipmap sets 
	//based on the largest magnitude coordinate direction (the major axis direction). 
	for (int i = 0; i < nCells; i++)	{
		int3 d = data[i];
		//int3 ret = XYZ2Idx(d, size);
		int idx = d.x* size2 + d.z * size + d.y;
		if (idx >= 0 && idx < size3)
			binCnt[idx] += 1;
	}

	vector<float> solAng = ComputePatchSolidAngle(size);

	const float den_uniform = nCells / (4 * M_PI);

	//const float scale = 0.03;	//for isabel
	//const float scale = 0.3;	//for plume
	for (int i = 0; i < size2; i++)	{
		for (int j = 0; j < 6; j++)	{
			int idx = j * size2 + i;
			cubemap[idx] = (float)binCnt[idx] / solAng[i] / den_uniform;// *scale;
		}
	}
}

inline float entropy(float *hist, int histlen){
	int i;
	float H;
	H = 0;
	for (i = 0; i<histlen; i++){
		if (hist[i] > 0)
			H -= (double)hist[i] * log2((double)hist[i]);
	}
	return H;
}

inline float CubemapEntropy(float *cubemap, int size)
{
	float sum = 0;
	int nBin = size * size * 6;
	for (int i = 0; i < nBin; i++)	{
		sum += cubemap[i];
	}
	std::unique_ptr<float[]> hist(new float[nBin]);
	for (int i = 0; i < nBin; i++)	{
		hist[i] = cubemap[i] / sum;
	}
	return entropy(hist.get(), nBin);
}

void DataManager::SplitTopNode()
{
	//$$$
	//Node left, right;
	for (int i = 0; i < 3; i++)	{
		blockDim[i] = ceil((float)dim[i] / initBlockSize);
	}
	int numBlocks = blockDim[0] * blockDim[1] * blockDim[2];
	Node **nd = new Node*[numBlocks];
	for (int i = 0; i < blockDim[0]; i++)	{
		for (int j = 0; j < blockDim[1]; j++)	{
			for (int k = 0; k < blockDim[2]; k++)	{
				int start[3] = { i * initBlockSize, j * initBlockSize, k * initBlockSize };
				int blockSize[3] = { 
					min(initBlockSize, dim[0] - i * initBlockSize),
					min(initBlockSize, dim[1] - j * initBlockSize),
					min(initBlockSize, dim[2] - k * initBlockSize) };
				nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k] = 
					new Node(start, blockSize, cubemap_size, 1);
			}
		}
	}
	float3* idata = static_cast<float3*>((void *)data);
	for (int i = 0; i < (blockDim[0] - 1); i++)	{
		for (int j = 0; j < blockDim[1]; j++)	{
			for (int k = 0; k < blockDim[2]; k++)	{
				Node* thisNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
				Node* nextNode = nd[(i + 1) * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
				int xa = thisNode->start[0] + thisNode->dim[0] - 1;
				int xb = xa + 2;
				int ya = thisNode->start[1];
				int yb = thisNode->start[1] + thisNode->dim[1];
				int za = thisNode->start[2];
				int zb = thisNode->start[2] + thisNode->dim[2];
				float sum = 0;
				for (int x = xa; x < xb; x++)	{
					for (int y = ya; y < yb; y++)	{
						for (int z = za; z < zb; z++)	{
							sum += idata[x * dim[1] * dim[2] + y * dim[2] + z].x;
						}
					}
				}
				if (sum > 0)
					thisNode->flux[1] = sum;
				else
					nextNode->flux[0] = - sum;
				thisNode->neighbor[1] = nextNode;
				nextNode->neighbor[0] = thisNode;
			}
		}
	}

	for (int i = 0; i < blockDim[0]; i++)	{
		for (int j = 0; j < (blockDim[1] - 1); j++)	{
			for (int k = 0; k < blockDim[2]; k++)	{
				Node* thisNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
				Node* nextNode = nd[i * blockDim[1] * blockDim[2] + (j + 1) * blockDim[2] + k];
				int xa = thisNode->start[0];
				int xb = thisNode->start[0] + thisNode->dim[0];
				int ya = thisNode->start[1] + thisNode->dim[1] - 1;
				int yb = ya + 2;
				int za = thisNode->start[2];
				int zb = thisNode->start[2] + thisNode->dim[2];
				float sum = 0;
				for (int x = xa; x < xb; x++)	{
					for (int y = ya; y < yb; y++)	{
						for (int z = za; z < zb; z++)	{
							sum += idata[x * dim[1] * dim[2] + y * dim[2] + z].y;
						}
					}
				}
				if (sum > 0)
					thisNode->flux[3] = sum;
				else
					nextNode->flux[2] = - sum;
				thisNode->neighbor[3] = nextNode;
				nextNode->neighbor[2] = thisNode;
			}
		}
	}

	for (int i = 0; i < blockDim[0]; i++)	{
		for (int j = 0; j < blockDim[1]; j++)	{
			for (int k = 0; k < (blockDim[2] - 1); k++)	{
				Node* thisNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k];
				Node* nextNode = nd[i * blockDim[1] * blockDim[2] + j * blockDim[2] + k + 1];
				int xa = thisNode->start[0];
				int xb = thisNode->start[0] + thisNode->dim[0];
				int ya = thisNode->start[1];
				int yb = thisNode->start[1] + thisNode->dim[1];
				int za = thisNode->start[2] + thisNode->dim[2] - 1;
				int zb = za + 2;
				float sum = 0;
				for (int x = xa; x < xb; x++)	{
					for (int y = ya; y < yb; y++)	{
						for (int z = za; z < zb; z++)	{
							sum += idata[x * dim[1] * dim[2] + y * dim[2] + z].z;
						}
					}
				}
				if (sum > 0)
					thisNode->flux[5] = sum;
				else
					nextNode->flux[4] = - sum;
				thisNode->neighbor[5] = nextNode;
				nextNode->neighbor[4] = thisNode;
			}
		}
	}

	for (int i = 0; i < numBlocks; i++)
	{
		ComputeCubemapNode(nd[i]);
		topNode->children.push_back(nd[i]);;
	}
	delete[] nd;
}

void DataManager::ComputeCubemapNode(Node *&nd)
{
	int blockDimTotal = nd->dim[0] * nd->dim[1] * nd->dim[2];
	std::unique_ptr<int3[]> datablock(new int3[blockDimTotal]);
	GetBlock(datablock.get(), nd->start[0], nd->start[1], nd->start[2], nd->dim[0], nd->dim[1], nd->dim[2]);
	nd->cubemap = new float[cubemap_size * cubemap_size * 6];
	ComputeCubeMap(datablock.get(), blockDimTotal, nd->cubemap, cubemap_size);
}

void DataManager::BuildOctree(Node *nd)
{
	//for (auto nd : node->children)	{
	float entropy = CubemapEntropy(nd->cubemap, nd->cube_size);
	nd->entropy = entropy;
	cout << entropy << ",";
	if (entropy > entropyThreshold)	{
		//Node *node0 = new Node(...);
		//BuildOctree(node0);
		/////....
		int childDimL[3], childDimR[3];
		childDimL[0] = nd->dim[0] / 2;
		childDimL[1] = nd->dim[1] / 2;
		childDimL[2] = nd->dim[2] / 2;

		childDimR[0] = nd->dim[0] - childDimL[0];
		childDimR[1] = nd->dim[1] - childDimL[1];
		childDimR[2] = nd->dim[2] - childDimL[2];

		Node *child;

		child = new Node(
			nd->start[0], nd->start[1], nd->start[2],
			childDimL[0], childDimL[1], childDimL[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0] + childDimL[0], nd->start[1], nd->start[2],
			childDimR[0], childDimL[1], childDimL[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0], nd->start[1] + childDimL[1], nd->start[2],
			childDimL[0], childDimR[1], childDimL[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0] + childDimL[0], nd->start[1] + childDimL[1], nd->start[2],
			childDimR[0], childDimR[1], childDimL[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0], nd->start[1], nd->start[2] + childDimL[2],
			childDimL[0], childDimL[1], childDimR[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0] + childDimL[0], nd->start[1], nd->start[2] + childDimL[2],
			childDimR[0], childDimL[1], childDimR[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0], nd->start[1] + childDimL[1], nd->start[2] + childDimL[2],
			childDimL[0], childDimR[1], childDimR[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);

		child = new Node(
			nd->start[0] + childDimL[0], nd->start[1] + childDimL[1], nd->start[2] + childDimL[2],
			childDimR[0], childDimR[1], childDimR[2],
			cubemap_size, nd->level + 1);
		ComputeCubemapNode(child);
		nd->children.push_back(child);
		BuildOctree(child);
	}
	else
	{
		numBlocks += 1;
	}
	//}
}

void DataManager::GetVolumeSize(int &nx, int &ny, int&nz)
{
	nx = dim[0];
	ny = dim[1];
	nz = dim[2];
}

int3 DataManager::GetVolumeDim()
{
	return make_int3(dim[0], dim[1], dim[2]);
}


void DataManager::GetQCube(int &x, int &y, int &z, int &nx, int &ny, int &nz)
{
	x = qCubePos[0];
	y = qCubePos[1];
	z = qCubePos[2];

	nx = qCubeSize[0];
	ny = qCubeSize[1];
	nz = qCubeSize[2];
}

float3 DataManager::GetQCubeCenter()
{
	return make_float3(
		qCubePos[0] + qCubeSize[0] * 0.5,
		qCubePos[1] + qCubeSize[1] * 0.5,
		qCubePos[2] + qCubeSize[2] * 0.5);
}

void DataManager::SetQCube(int x, int y, int z, int nx, int ny, int nz)
{
	qCubePos[0] = x;
	qCubePos[1] = y;
	qCubePos[2] = z;

	qCubeSize[0] = nx;
	qCubeSize[1] = ny;
	qCubeSize[2] = nz;
}

bool DataManager::CubeInsideVolume(int x, int y, int z, int nx, int ny, int nz)
{
	return x >= 0 && y >= 0 && z >= 0 && (x + nx) <= dim[0] && (y + ny) <= dim[1] && (z + nz) <= dim[2];
}

bool DataManager::CubeInsideVolumeX(int x, int nx)
{
	return x >= 0 && (x + nx) <= dim[0];
}

bool DataManager::CubeInsideVolumeY(int x, int nx)
{
	return x >= 0 && (x + nx) <= dim[1];
}

bool DataManager::CubeInsideVolumeZ(int x, int nx)
{
	return x >= 0 && (x + nx) <= dim[2];
}

void DataManager::MoveCube(int x, int y, int z)
{
	if (!CubeInsideVolumeX(qCubePos[0] + x, qCubeSize[0]))
		x = 0;
	if (!CubeInsideVolumeY(qCubePos[1] + y, qCubeSize[1]))
		y = 0;
	if (!CubeInsideVolumeZ(qCubePos[2] + z, qCubeSize[2]))
		z = 0;
	qCubePos[0] += x;
	qCubePos[1] += y;
	qCubePos[2] += z;
}

void DataManager::ResizeCube(int x, int y, int z)
{
	if (!CubeInsideVolumeX(qCubePos[0], qCubeSize[0] + x) || (qCubeSize[0] + x < 0))
		x = 0;
	if (!CubeInsideVolumeY(qCubePos[1], qCubeSize[1] + y) || (qCubeSize[1] + y < 0))
		y = 0;
	if (!CubeInsideVolumeZ(qCubePos[2], qCubeSize[2] + z) || (qCubeSize[2] + z < 0))
		z = 0;
	qCubeSize[0] += x;
	qCubeSize[1] += y;
	qCubeSize[2] += z;
}


void DataManager::LoadVec(char* filename)
{
	using namespace std;
	FILE* file;
	file = fopen(filename, "rb");

	fread(dim, sizeof(int), 3, file);
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
	initBlockSize = min(min(dim[0], dim[1]), dim[2]) / 3;



	fclose(file);
	cout << "Data loaded" << endl;

	qCubePos[0] = 0;
	qCubePos[1] = 0;
	qCubePos[2] = 0;

	qCubeSize[0] = dim[0];
	qCubeSize[1] = dim[1];
	qCubeSize[2] = dim[2];

	cubemap_size = 16;

	IndexVolume(cubemap_size);

	int start[3] = {0, 0, 0};
	topNode = new Node(start, dim, cubemap_size, 0);
	//ComputeCurl();

	LoadOSUFlow(filename);
}

void DataManager::LoadOSUFlow(char* filename)
{
	list<vtListSeedTrace*> sl_list;

	// load the scalar field
//	LOG(printf("read file %s\n", argv[1]));

	osuflow->LoadData(filename, true); //true: a steady flow field 

	////szVecFilePath = argv[1];	// ADD-BY-LEETEN 09/29/2012

	//// comptue the bounding box of the streamlines 
	//VECTOR3 minB, maxB;
	//VECTOR3 minLen, maxLen;
	//float center[3], len[3];
	//osuflow->Boundary(minLen, maxLen); // get the boundary 
	//minB[0] = minLen[0]; minB[1] = minLen[1];  minB[2] = minLen[2];
	//maxB[0] = maxLen[0]; maxB[1] = maxLen[1];  maxB[2] = maxLen[2];
	////  osuflow->SetBoundary(minB, maxB);  // set the boundary. just to test
	//// the subsetting feature of OSUFlow
	//printf(" volume boundary X: [%f %f] Y: [%f %f] Z: [%f %f]\n",
	//	minLen[0], maxLen[0], minLen[1], maxLen[1],
	//	minLen[2], maxLen[2]);

	//center[0] = (minLen[0] + maxLen[0]) / 2.0;
	//center[1] = (minLen[1] + maxLen[1]) / 2.0;
	//center[2] = (minLen[2] + maxLen[2]) / 2.0;
	//printf("center is at %f %f %f \n", center[0], center[1], center[2]);
	//len[0] = maxLen[0] - minLen[0];
	//len[1] = maxLen[1] - minLen[1];
	//len[2] = maxLen[2] - minLen[2];


	//float from[3], to[3];

	//from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2];
	//to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2];

	//printf("generating seeds...\n");
	//osuflow->SetRandomSeedPoints(from, to, 100);
	//int nSeeds;
	//VECTOR3* seeds = osuflow->GetSeeds(nSeeds);
	//for (int i = 0; i<nSeeds; i++)
	//	printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0],
	//	seeds[i][1], seeds[i][2]);

	//sl_list.clear();

	//printf("compute streamlines..\n");
	//osuflow->SetIntegrationParams(1, 5);
	////osuflow->GenStreamLines(sl_list, BACKWARD_AND_FORWARD, 200, 0);
	//osuflow->GenStreamLines(sl_list, FORWARD_DIR, 100, 0);
	//printf(" done integrations\n");
	//printf("list size = %d\n", (int)sl_list.size());

	//int iT = 0;
	//for (list<vtListSeedTrace*>::const_iterator
	//	pIter = sl_list.begin();
	//	pIter != sl_list.end();
	//pIter++, iT++)
	//{
	//	const vtListSeedTrace *trace = *pIter;
	//	int iP = 0;
	//	vector<float4> line;
	//	for (list<VECTOR3*>::const_iterator
	//		pnIter = trace->begin();
	//		pnIter != trace->end();
	//	pnIter++, iP++)
	//	{
	//		VECTOR3 p = **pnIter;
	//		line.push_back(make_float4(p[0], p[1], p[2], 1.0f));
	//	}
	//	streamlines.push_back(line);
	//}
}

void DataManager::GenStreamInCube()
{
	printf("generating seeds...\n");
	list<vtListSeedTrace*> sl_list;
	float from[3], to[3];
	//int qCubePos[3], qCubeSize[3];	//queried cube position and sizes

	from[0] = qCubePos[0];   from[1] = qCubePos[1];   from[2] = qCubePos[2];
	to[0] = qCubePos[0] + qCubeSize[0];   to[1] = qCubePos[1] + qCubeSize[1];   to[2] = qCubePos[2] + qCubeSize[2];

	osuflow->SetRandomSeedPoints(from, to, 16);
	int nSeeds;
	VECTOR3* seeds = osuflow->GetSeeds(nSeeds);
	for (int i = 0; i<nSeeds; i++)
		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0],
		seeds[i][1], seeds[i][2]);

	sl_list.clear();

	printf("compute streamlines..\n");
	osuflow->SetIntegrationParams(1, 5);
	//osuflow->GenStreamLines(sl_list, BACKWARD_AND_FORWARD, 50, 0);
	osuflow->GenStreamLines(sl_list, FORWARD_DIR, 100, 0);
	printf(" done integrations\n");
	printf("list size = %d\n", (int)sl_list.size());

	int iT = 0;
	streamlinesInCube.clear();
	for (list<vtListSeedTrace*>::const_iterator
		pIter = sl_list.begin();
		pIter != sl_list.end();
	pIter++, iT++)
	{
		const vtListSeedTrace *trace = *pIter;
		int iP = 0;
		vector<float4> line;
		for (list<VECTOR3*>::const_iterator
			pnIter = trace->begin();
			pnIter != trace->end();
		pnIter++, iP++)
		{
			VECTOR3 p = **pnIter;
			line.push_back(make_float4(p[0], p[1], p[2], 1.0f));
		}
		streamlinesInCube.push_back(line);
	}
}


void DataManager::GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz)
{
	//std::cout << "z:" << z << std::endl;
	//using namespace std;
	std::vector<int3> ret;
	//int cnt4 = 0;
	//int3* idata = static_cast<int3*>((void *)data);
	for (int i = 0; i < nx; i++)	{
		int ix = i + x;
		for (int j = 0; j < ny; j++)	{
			int iy = j + y;
			for (int k = 0; k < nz; k++)	{
				int iz = k + z;
				int idx = ix * dim[1] * dim[2] + iy * dim[2] + iz;
				int3 d = dataIdx[idx];
				//if (d.x == 0 && d.y == 0 && d.z == 0)
				//{
				//	cnt4++;
				//	//std::cout<<"iii:" << ix << "," << iy << "," << iz << "," << idx <<std::endl;
				//}
				//ret.push_back();
				datablock[i * ny * nz + j * nz + k] = dataIdx[ix * dim[1] * dim[2] + iy * dim[2] + iz];
			}
		}
	}
	//std::cout << "cnt4:" << cnt4 << std::endl;
	//return ret;
}

void DataManager::GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap)
{
	qCubePos[0] = x;
	qCubePos[1] = y;
	qCubePos[2] = z;
	qCubeSize[0] = nx;
	qCubeSize[1] = ny;
	qCubeSize[2] = nz;
	cubemap = new float[cubemap_size * cubemap_size * 6];
//	std::vector<float3> datablock = GetBlock(x, y, z, nx, ny, nz);
//	ComputeCubeMap(dataIdx, GetNumOfCells(), cubemap, size);
	UpdateCubeMap(cubemap);
	cubemap_data = cubemap;
//	cubemap_size = size;

}

void DataManager::IndexVolume(int size)
{
	float3* idata = static_cast<float3*>((void *)data);
	int nCells = GetNumOfCells();
	dataIdx = new int3[nCells];
	//for (int i = 0; i < GetNumOfCells(); i++)
	//{
	//	dataIdx[i] = make_int3(2,2,2);
	//}
	//int cnt = 0;
	//for (int i = 0; i < dim[0]; i++)	{
	//	for (int j = 0; j < dim[1]; j++)	{
	//		for (int k = 0; k < dim[2]; k++)	{
	//			int idx = i * dim[1] * dim[2] + j * dim[2] + k;
	//			float3 d = idata[idx];
	//			dataIdx[idx] = XYZ2Idx(d, size);
	//			//int3 tmp = dataIdx[i * dim[1] * dim[2] + j * dim[2] + k];
	//			//if (tmp.x == 0 && tmp.y == 0 && tmp.z == 0)
	//			//{
	//			//	//std::cout << "d = (" << d.x << "," << d.y << "," << d.z << ")" << std::endl;
	//			//	cnt++;
	//			//}
	//		}
	//	}
	//}
	for (int i = 0; i < nCells; i++)
		dataIdx[i] = XYZ2Idx(idata[i], cubemap_size);

	//int cnt3 = 0;
	//for (int i = 0; i < GetNumOfCells(); i++)
	//{
	//	int3 d = dataIdx[i];
	//	if (d.x == 0 && d.y == 0 && d.z == 0)
	//		cnt3++;
	//}
	//std::cout << "cnt:" << cnt << std::endl;
	//std::cout << "cnt3:" << cnt3 << std::endl;
}

void DataManager::ComputeCurl()
{
	float3* idata = static_cast<float3*>((void *)data);
	curlIdx = new int3[dim[0] * dim[1] * dim[2]];

	for (int i = 0; i < dim[0]; i++)	{
		for (int j = 0; j < dim[1]; j++)	{
			for (int k = 0; k < dim[2]; k++)	{
				if (0 == i || 0 == j || 0 == k || (dim[0] - 1) == i || (dim[1] - 1) == j || (dim[2] - 1) == k)
					curlIdx[i * dim[1] * dim[2] + j * dim[2] + k] = make_int3(0, 0, 0);
				else
				{
					float3 dx1	= idata[(i + 1) * dim[1] * dim[2] + j * dim[2] + k];
					float3 dx_1 = idata[(i - 1) * dim[1] * dim[2] + j * dim[2] + k];
					
					float3 dy1	= idata[i * dim[1] * dim[2] + (j + 1) * dim[2] + k];
					float3 dy_1 = idata[i * dim[1] * dim[2] + (j - 1) * dim[2] + k];
					
					float3 dz1	= idata[i * dim[1] * dim[2] + j * dim[2] + k + 1];
					float3 dz_1 = idata[i * dim[1] * dim[2] + j * dim[2] + k - 1];

					float3 curl = make_float3(
						(dy1.z - dy_1.z) - (dz1.y - dz_1.y), 
						(dz1.x - dz_1.x) - (dx1.z - dx_1.z), 
						(dx1.y - dx_1.y) - (dy1.x - dy_1.x));
						//(dz1.y - dz_1.y) - (dy1.z - dy_1.z),
						//(dx1.z - dx_1.z) - (dz1.x - dz_1.x),
						//(dy1.x - dy_1.x) - (dx1.y - dx_1.y));

					if ((curl.x * curl.x + curl.y * curl.y + curl.z * curl.z) > 2000)
						curlIdx[i * dim[1] * dim[2] + j * dim[2] + k] = XYZ2Idx(curl, cubemap_size);
					else
						curlIdx[i * dim[1] * dim[2] + j * dim[2] + k] = make_int3(0, 0, 0);
				}
			}
		}
	}

}

int DataManager::GetCubemapSize()
{
	return cubemap_size;
}

int DataManager::GetNumOfCells()
{
	return dim[0] * dim[1] * dim[2];
}


void DataManager::QueryByBin(int f, int x, int y, unsigned char* result)
{
	//$$$
	// isabel: 2, 3
	// isabel, curl: 4
	// plume: 4, 5
	f = 4;
	int a = 4;
	int x1 = cubemap_size / 2 - a, y1 = cubemap_size / 2 - a;
	int x2 = cubemap_size / 2 + a, y2 = cubemap_size / 2 + a;
	int cnt = 0;
	for (int i = 0; i < dim[0]; i++)	{
		for (int j = 0; j < dim[1]; j++)	{
			for (int k = 0; k < dim[2]; k++)	{
				//$$$
				int3 b = dataIdx[i * dim[1] * dim[2] + j * dim[2] + k];
				//int3 b = curlIdx[i * dim[1] * dim[2] + j * dim[2] + k];
				if (b.x == f && b.y > x1 && b.y < x2 && b.z > y1 && b.z < y2)
				//if (i < (dim[2] / 2))
				{
					result[i * dim[1] * dim[2] + j * dim[2] + k] = (unsigned char)255;
					cnt++;
				}
				else
					result[i * dim[1] * dim[2] + j * dim[2] + k] = (unsigned char)0;
			}
		}
	}
	std::cout << "perc:" << (float) cnt / (dim[0] * dim[1] * dim[2])<< std::endl;
}


void DataManager::UpdateCubeMap(float* cubemap)
{
	//qCubePos[0] = x;
	//qCubePos[1] = y;
	//qCubePos[2] = z;
	//qCubeSize[0] = nx;
	//qCubeSize[1] = ny;
	//qCubeSize[2] = nz;
	int cubeSizeTotal = qCubeSize[0] * qCubeSize[1] * qCubeSize[2];
	std::unique_ptr<int3[]> datablock(new int3[cubeSizeTotal]);

	GetBlock(datablock.get(), qCubePos[0], qCubePos[1], qCubePos[2], qCubeSize[0], qCubeSize[1], qCubeSize[2]);
	//int cnt = 0;
	//for (int i = 0; i < datablock.size(); i++)
	//{
	//	int3 tmp = datablock[i];
	//	if (tmp.x == 0 && tmp.y == 0 && tmp.z == 0)
	//		cnt++;
	//}
	//std::cout << "cnt 2 ::" << cnt << std::endl;
	//if (datablock.size() != (qCubeSize[0] * qCubeSize[1] * qCubeSize[2]))
	//{
	//	std::cout << "error" << std::endl;
	//	exit(1);
	//}
	ComputeCubeMap(datablock.get(), cubeSizeTotal, cubemap, cubemap_size);
}

DataManager::DataManager()
{
	numBlocks = 0;
}

DataManager::~DataManager()
{
	delete[] cubemap_data;
	delete[] data;
	delete[] data_x_first;
	delete[] dataIdx;
	delete[] curlIdx;
	delete[] topNode;
}

std::vector<std::vector<double> > DataManager::GetVertexPos()
{
	return vV;
}

std::vector<std::vector<int> > DataManager::GetFaceVertices()
{
	return vF;
}

std::vector<float> DataManager::GetVertexValue()
{
	return vertexValue;
}

std::vector<std::vector<double> > DataManager::GetScaledVertexPos()
{
	return vVScaled;
}

void DataManager::Segmentation()
{
	//topNode = new Node(cubemap_data, cubemap_size, dataIdx, dim);
	SplitTopNode();
	//uncomment later
	for (auto child : topNode->children)
	{
		BuildOctree(child);
	}
	cout << "number of cubic blocks: " << numBlocks << endl;
}

vector<Node*> DataManager::GetAllNode()
{
	vector<Node*> ret;
	GetDescendantNodes(ret, topNode);
	return ret;
}

void DataManager::GetDescendantNodes(vector<Node*> &ret, Node* nd)
{
	for (auto child : nd->children){
		if (child->children.size() > 0)
			GetDescendantNodes(ret, child);
		else
			ret.push_back(child);
	}
}

vector<vector<float4>> DataManager::GetStreamlines()
{
	return streamlines;
}

vector<vector<float4>> DataManager::GetStreamlinesInCube()
{
	return streamlinesInCube;
}

float* DataManager::GetVecData()
{
	return data;
}

float* DataManager::GetVecDataXFirst()
{
	return (float*)data_x_first;
}

