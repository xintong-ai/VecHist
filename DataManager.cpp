#include "DataManager.h"
#include <stdio.h>
#include <iostream>
#include "dstat.h"
#include <memory>

void DataManager::GetVolumeSize(int &nx, int &ny, int&nz)
{
	nx = dim[0];
	ny = dim[1];
	nz = dim[2];
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
	//using namespace std;
	std::vector<float3> ret;
	float3* idata = static_cast<float3*>((void *)data);
	for (int i = 0; i < nx; i++)	{
		int ix = i + x;
		for (int j = 0; j < ny; j++)	{
			int iy = j + y;
			for (int k = 0; k < nz; k++)	{
				int iz = k + z;
				ret.push_back(idata[ix * ny * nz + iy * nz + iz]);
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

inline void ComputeCubeMap(std::vector<float3> data, float* cubemap, const int size)
{
	using namespace std;
	vector<int> binCnt(size * size * 6, 0);// (new int(size * size * 6));
	//int *binCnt = new int[size * size * 6];
	const int size2 = size * size;

	float x, y;
	int binx, biny;
	
	for (auto d : data)	{
		int f = -1;
		if		(d.x >= abs(d.y) && d.x > abs(d.z))
			f = 0;
		else if (d.x < - abs(d.y) && d.x <= - abs(d.z))
			f = 1;
		else if (d.y >= abs(d.z) && d.y > abs(d.x))
			f = 2;
		else if (d.y < - abs(d.z) && d.y <= - abs(d.x))
			f = 3;
		else if (d.z >= abs(d.x) && d.z > abs(d.y))
			f = 4;
		else if (d.z < - abs(d.x) && d.z <= - abs(d.y))
			f = 5;
		else 
			continue;

		//switch (f)
		//{
		//case 0:
		//	binCnt
		//	break;
		//}
		int ci = f / 2;
		x = (d[(ci + 1) % 3]) / d[ci];
		y = (d[(ci + 2) % 3]) / d[ci];

		float binSize = 2.0 / size;
		binx = (x + 1) / binSize;
		biny = (y + 1) / binSize;

		binCnt[f* size2 + binx * size + biny] += 1;
	}

	vector<float> solAng = ComputePatchSolidAngle(size);

	const float den_uniform = data.size() / (4 * M_PI);

	for (int i = 0; i < size2; i++)	{
		for (int j = 0; j < 6; j++)	{
			int idx = j * size2 + i;
			cubemap[idx] = (float)binCnt[idx] / solAng[i] / den_uniform;
		}
	}
}

void DataManager::GenCubeMap(int x, int y, int z, int nx, int ny, int nz, float* cubemap, int size)
{
	std::vector<float3> datablock = GetBlock(x, y, z, nx, ny, nz);
	ComputeCubeMap(datablock, cubemap, size);
}

DataManager::~DataManager()
{
	delete [] data;
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