#include "Cubemap.h"
#include <vector>
#include <vector_types.h>
#include <vector_functions.h>
#include <memory>


double Log2(double n)
{
	// log(n)/log(2) is log2.  
	return log(n) / log(2);
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

	//const float den_uniform = nCells / (4 * M_PI);

	//const float scale = 0.03;	//for isabel
	//const float scale = 0.3;	//for plume
	float scale_factor = 1000;
	for (int i = 0; i < size2; i++)	{
		for (int j = 0; j < 6; j++)	{
			int idx = j * size2 + i;
			cubemap[idx] = (float)binCnt[idx] / (solAng[i] * scale_factor);// / den_uniform;// *scale;
		}
	}
	float sum = 0;

	//normalize the histogram
	for (int i = 0; i < size3; i++)
		sum += cubemap[i];

	for (int i = 0; i < size3; i++)
		cubemap[i] /= (sum / scale_factor);
}

inline float entropy(float *hist, int histlen){
	int i;
	float H;
	H = 0;
	for (i = 0; i<histlen; i++){
		if (hist[i] > 0)
			H -= (double)hist[i] * Log2((double)hist[i]);
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

Cubemap::Cubemap()
{
	qCubePos[0] = 0;
	qCubePos[1] = 0;
	qCubePos[2] = 0;

	qCubeSize[0] = dim[0];
	qCubeSize[1] = dim[1];
	qCubeSize[2] = dim[2];

	cubemap_size = 16;
	IndexVolume(cubemap_size);

}


float3 Cubemap::GetQCubeCenter()
{
	return make_float3(
		qCubePos[0] + qCubeSize[0] * 0.5,
		qCubePos[1] + qCubeSize[1] * 0.5,
		qCubePos[2] + qCubeSize[2] * 0.5);
}

void Cubemap::ResizeCube(int x, int y, int z)
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


void Cubemap::IndexVolume(int size)
{
	float3* idata = static_cast<float3*>((void *)data);
	int nCells = dim[0] * dim[1] * dim[2];// GetNumOfCells();
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

bool Cubemap::CubeInsideVolumeX(int x, int nx)
{
	return x >= 0 && (x + nx) <= dim[0];
}

bool Cubemap::CubeInsideVolumeY(int x, int nx)
{
	return x >= 0 && (x + nx) <= dim[1];
}

bool Cubemap::CubeInsideVolumeZ(int x, int nx)
{
	return x >= 0 && (x + nx) <= dim[2];
}

void Cubemap::GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* &cubemap)
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

void Cubemap::UpdateCubeMap(float* cubemap)
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


void Cubemap::GetBlock(int3* datablock, int x, int y, int z, int nx, int ny, int nz)
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

//void VecReader::ComputeCurl()
//{
//	float3* idata = static_cast<float3*>((void *)data);
//	curlIdx = new int3[dim[0] * dim[1] * dim[2]];
//
//	for (int i = 0; i < dim[0]; i++)	{
//		for (int j = 0; j < dim[1]; j++)	{
//			for (int k = 0; k < dim[2]; k++)	{
//				if (0 == i || 0 == j || 0 == k || (dim[0] - 1) == i || (dim[1] - 1) == j || (dim[2] - 1) == k)
//					curlIdx[i * dim[1] * dim[2] + j * dim[2] + k] = make_int3(0, 0, 0);
//				else
//				{
//					float3 dx1 = idata[(i + 1) * dim[1] * dim[2] + j * dim[2] + k];
//					float3 dx_1 = idata[(i - 1) * dim[1] * dim[2] + j * dim[2] + k];
//
//					float3 dy1 = idata[i * dim[1] * dim[2] + (j + 1) * dim[2] + k];
//					float3 dy_1 = idata[i * dim[1] * dim[2] + (j - 1) * dim[2] + k];
//
//					float3 dz1 = idata[i * dim[1] * dim[2] + j * dim[2] + k + 1];
//					float3 dz_1 = idata[i * dim[1] * dim[2] + j * dim[2] + k - 1];
//
//					float3 curl = make_float3(
//						(dy1.z - dy_1.z) - (dz1.y - dz_1.y),
//						(dz1.x - dz_1.x) - (dx1.z - dx_1.z),
//						(dx1.y - dx_1.y) - (dy1.x - dy_1.x));
//					//(dz1.y - dz_1.y) - (dy1.z - dy_1.z),
//					//(dx1.z - dx_1.z) - (dz1.x - dz_1.x),
//					//(dy1.x - dy_1.x) - (dx1.y - dx_1.y));
//
//					if ((curl.x * curl.x + curl.y * curl.y + curl.z * curl.z) > 2000)
//						curlIdx[i * dim[1] * dim[2] + j * dim[2] + k] = XYZ2Idx(curl, cubemap_size);
//					else
//						curlIdx[i * dim[1] * dim[2] + j * dim[2] + k] = make_int3(0, 0, 0);
//				}
//			}
//		}
//	}
//
//}

void Cubemap::QueryByBin(int f, int x, int y, unsigned char* result)
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
#ifdef DEBUG_PRINTING
	std::cout << "perc:" << (float)cnt / (dim[0] * dim[1] * dim[2]) << std::endl;
#endif
}

Cubemap::~Cubemap()
{
	delete[] cubemap_data;

}