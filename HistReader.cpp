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
//#include <iostream>
#include <vector>


using namespace std;


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

	LoadVec("D:/data/plume/15plume3d421-504x504x2048.vec");// GetStringVal("vectorfield").c_str());
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

	data = new float[dimtotal * 3];

	fread(data, sizeof(float3), dimtotal, file);


	fclose(file);
	cout << "Data loaded" << endl;
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
}

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

