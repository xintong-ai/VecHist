#include "DataManager.h"
#include <stdio.h>
#include <iostream>
//#include "dstat.h"
#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector_functions.h>

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

	cubemap_size = 32;

	IndexVolume(cubemap_size);
	ComputeCurl();
//
//	int block_size = 51/*atoi(argv[5])*/, block_size_it = block_size - 1, dimbs[3];
//	dimbs[0] = static_cast<int>(ceil(static_cast<float>(dims[0]) / block_size_it));
//	dimbs[1] = static_cast<int>(ceil(static_cast<float>(dims[1]) / block_size_it));
//	dimbs[2] = static_cast<int>(ceil(static_cast<float>(dims[2]) / block_size_it));
//	int dimbtotal = dimbs[0] * dimbs[1] * dimbs[2];
//
//	vector<int> bn; vector<float*> sg;
//	dstat::gen_spgrid(1, &bn, &sg, vV, vF);
//	int bnf = *(bn.end() - 1);
//	float* sgf = *(sg.end() - 1);
//
//	float** datah = new float*[dimbtotal];
//	//for (int i = 0; i != dimbtotal; ++i)
//		//dstat::gen_hist(data, dims, dimbs, bn, sg, block_size, i, datah[i]);
//	const int selectedBlock = 20;
//	dstat::gen_hist(data, dims, dimbs, bn, sg, block_size, selectedBlock, datah[selectedBlock]);
//
//	//const int nF = 20;	//number of faces
//	vertexValue.assign(vV.size(), 0.1);
//	for (int i = 0; i < vF.size(); i++)	{
//		for (int j = 0; j < 3; j++)	{
//			vertexValue[vF[i][j]] += datah[selectedBlock][i];
//		}
//	}
//
//	for (int i = 0; i < vV.size(); i++)	{
//		float s = vertexValue[i];
//		vVScaled.push_back(vector<double>{vV[i][0] * s, vV[i][1] * s, vV[i][2] * s});
//	}
//	Eigen::MatrixXd V;
//	Eigen::MatrixXi F;
////	igl::readOFF("D:/lib/libigl/tutorial/shared/bunny.off", V, F);
//	igl::list_to_matrix(vVScaled, V);
//	igl::list_to_matrix(vF, F);
//	// Plot the mesh
//	igl::Viewer viewer;
//	viewer.data.set_mesh(V, F);
//	viewer.launch();
//
	// output histogram data
	//dstat::log_histogram_data(datah, dimbtotal, bnf,
	//	"isabel_16.hist");
}

std::vector<float3> DataManager::GetBlock(int x, int y, int z, int nx, int ny, int nz)
{
	std::cout << "z:" << z << std::endl;
	//using namespace std;
	std::vector<float3> ret;
	float3* idata = static_cast<float3*>((void *)data);
	for (int i = 0; i < nx; i++)	{
		int ix = i + x;
		for (int j = 0; j < ny; j++)	{
			int iy = j + y;
			for (int k = 0; k < nz; k++)	{
				int iz = k + z;
				ret.push_back(idata[ix * dim[1] * dim[2] + iy * dim[2] + iz]);
			}
		}
	}
	return ret;
}

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
	int f = 0;
	float x = 0, y = 0;
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
	//	continue;

	return make_int3(f, x * size, y * size);

}

inline void ComputeCubeMap(std::vector<float3> data, float* cubemap, const int size)
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
	for (auto d : data)	{
		int3 ret = XYZ2Idx(d, size);
		int idx = ret.x* size2 + ret.z * size + ret.y;
		if (idx >= 0 && idx < size3)
			binCnt[idx] += 1;
	}

	vector<float> solAng = ComputePatchSolidAngle(size);

	const float den_uniform = data.size() / (4 * M_PI);

	//const float scale = 0.03;	//for isabel
	const float scale = 0.3;	//for plume
	for (int i = 0; i < size2; i++)	{
		for (int j = 0; j < 6; j++)	{
			int idx = j * size2 + i;
			cubemap[idx] = (float)binCnt[idx] / solAng[i] / den_uniform * scale;
		}
	}
}

void DataManager::GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap, int size)
{
	qCubePos[0] = x;
	qCubePos[1] = y;
	qCubePos[2] = z;
	qCubeSize[0] = nx;
	qCubeSize[1] = ny;
	qCubeSize[2] = nz;
	cubemap = new float[size * size * 6];
	std::vector<float3> datablock = GetBlock(x, y, z, nx, ny, nz);
	ComputeCubeMap(datablock, cubemap, size);
	cubemap_data = cubemap;
	cubemap_size = size;
}

void DataManager::IndexVolume(int size)
{
	float3* idata = static_cast<float3*>((void *)data);
	dataIdx = new int3[dim[0] * dim[1] * dim[2]];

	for (int i = 0; i < dim[0]; i++)	{
		for (int j = 0; j < dim[1]; j++)	{
			for (int k = 0; k < dim[2]; k++)	{
				float3 d = idata[i * dim[1] * dim[2] + j * dim[2] + k];
				dataIdx[i * dim[1] * dim[2] + j * dim[2] + k] = XYZ2Idx(d, size);
			}
		}
	}
}

void DataManager::ComputeCurl()
{
	float3* idata = static_cast<float3*>((void *)data);
	curlIdx = new int3[dim[0] * dim[1] * dim[2]];

	for (int i = 0; i < dim[0]; i++)	{
		for (int j = 0; j < dim[1]; j++)	{
			for (int k = 0; k < dim[2]; k++)	{
				if (0 == i || 0 == j || 0 == k || (dim[0] - 1) == i || (dim[1] - 1) == j || (dim[2] - 1) == k)
					dataIdx[i * dim[1] * dim[2] + j * dim[2] + k] = make_int3(0, 0, 0);
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
	// isabel: 2, 3
	// isabel, curl: 4
	// plume: 4, 5
	f = 2;
	int a = 4;
	int x1 = 16 - a, y1 = 16 - a;
	int x2 = 16 + a, y2 = 16 + a;
	int cnt = 0;
	for (int i = 0; i < dim[0]; i++)	{
		for (int j = 0; j < dim[1]; j++)	{
			for (int k = 0; k < dim[2]; k++)	{
				int3 b = dataIdx[i * dim[1] * dim[2] + j * dim[2] + k];
				//int3 b = curlIdx[i * dim[1] * dim[2] + j * dim[2] + k];
				if (b.x == f && b.y > x1 && b.y < x2 && b.z > y1 && b.z < y2)
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


void DataManager::UpdateCubeMap(float* cubemap, int size)
{
	//qCubePos[0] = x;
	//qCubePos[1] = y;
	//qCubePos[2] = z;
	//qCubeSize[0] = nx;
	//qCubeSize[1] = ny;
	//qCubeSize[2] = nz;
	std::vector<float3> datablock = GetBlock(qCubePos[0], qCubePos[1], qCubePos[2], qCubeSize[0], qCubeSize[1], qCubeSize[2]);
	ComputeCubeMap(datablock, cubemap, size);
}


DataManager::~DataManager()
{
	delete [] cubemap_data;
	delete [] data;
	delete [] dataIdx;
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