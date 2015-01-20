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
	initBlockSize = 64;
	//Node left, right;
	for (int i = 0; i < 3; i++)	{
		blockDim[i] = ceil((float)dim[i] / initBlockSize);
	}
	for (int i = 0; i < blockDim[0]; i++)	{
		for (int j = 0; j < blockDim[1]; j++)	{
			for (int k = 0; k < blockDim[2]; k++)	{
				int start[3] = { i * initBlockSize, j * initBlockSize, k * initBlockSize };
				int blockDim[3] = { 
					min(initBlockSize, dim[0] - i * initBlockSize),
					min(initBlockSize, dim[1] - j * initBlockSize),
					min(initBlockSize, dim[2] - k * initBlockSize) };
				Node *nd = new Node(start, blockDim, cubemap_size);
				ComputeCubemapNode(nd);
				topNode->children.push_back(nd);
			}
		}
	}
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
		//cout << "entropy:\t" << entropy << endl;
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
				childDimL[0],					childDimL[1],					childDimL[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0] + childDimL[0], nd->start[1], nd->start[2],
				childDimR[0],					childDimL[1],					childDimL[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0], nd->start[1] + childDimL[1], nd->start[2],
				childDimL[0],					childDimR[1],					childDimL[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0] + childDimL[0], nd->start[1] + childDimL[1], nd->start[2],
				childDimR[0],					childDimR[1],					childDimL[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0], nd->start[1], nd->start[2] + childDimL[2],
				childDimL[0],					childDimL[1],					childDimR[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0] + childDimL[0], nd->start[1], nd->start[2] + childDimL[2],
				childDimR[0],					childDimL[1],					childDimR[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0], nd->start[1] + childDimL[1], nd->start[2] + childDimL[2],
				childDimL[0],					childDimR[1],					childDimR[2],
				cubemap_size);
			ComputeCubemapNode(child);
			nd->children.push_back(child);
			BuildOctree(child);

			child = new Node(
				nd->start[0] + childDimL[0], nd->start[1] + childDimL[1], nd->start[2] + childDimL[2],
				childDimR[0],					childDimR[1],					childDimR[2],
				cubemap_size);
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

void DataManager::GetQCube(int &x, int &y, int &z, int &nx, int &ny, int &nz)
{
	x = qCubePos[0];
	y = qCubePos[1];
	z = qCubePos[2];

	nx = qCubeSize[0];
	ny = qCubeSize[1];
	nz = qCubeSize[2];
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

void DataManager::LoadVec(char* filename)
{
	using namespace std;
	FILE* file;
	file = fopen(filename, "rb");

	fread(dim, sizeof(int), 3, file);
	int dimtotal = dim[0] * dim[1] * dim[2];

	data = new float[dimtotal * 3];
	fread(data, sizeof(float), dimtotal * 3, file);

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
	topNode = new Node(start, dim, cubemap_size);
	//ComputeCurl();
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
	delete [] cubemap_data;
	delete [] data;
	delete [] dataIdx;
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
	for (auto child : topNode->children)
	{
		BuildOctree(child);
	}
	cout << "number of cubic blocks: " << numBlocks << endl;
}