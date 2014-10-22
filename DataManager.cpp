#include "DataManager.h"
#include <stdio.h>
#include <iostream>
#include "dstat.h"


void DataManager::LoadVec(char* filename)
{
	using namespace std;
	FILE* file;
	file = fopen(filename, "rb");

	int dims[3];
	fread(dims, sizeof(int), 3, file);
	int dimtotal = dims[0] * dims[1] * dims[2];

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