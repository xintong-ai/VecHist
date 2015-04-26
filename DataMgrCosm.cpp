#include "DataMgrCosm.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

DataMgrCosm::DataMgrCosm()
{
}

DataMgrCosm::~DataMgrCosm()
{
}

void DataMgrCosm::LoadData()
{
	cubemap_size = 16;
	//TODO: remove the hard coded part
	//dim[0] = 62.5;
	//dim[1] = 62.5;
	//dim[2] = 62.5;

	LoadHalos();
}

void DataMgrCosm::LoadHalos()
{
	
	using namespace std;
	ifstream fin(GetStringVal("halo").c_str());
	assert(!fin.fail());
	float data;
	//fin >> data;        // get first number from the file (priming the input statement)
	string line;
	std::getline(fin, line);
	stringstream ss(line);

	float tmp;

	ss >> tmp;
	start[0] = tmp;
	
	ss >> tmp;
	dim[0] = tmp;
	
	ss >> tmp;
	start[1] = tmp;
	
	ss >> tmp;
	dim[1] = tmp;
	
	ss >> tmp;
	start[2] = tmp;
	
	ss >> tmp;
	dim[2] = tmp;

	std::getline(fin, line);

	// You must attempt to read info prior to an eof( ) test.
	while (!fin.eof())      //if not at end of file, continue reading numbers
	{
		stringstream ss(line);
//		cout << line << endl;    //print numbers to screen
		float pos[3];
		float eigval[3];
		float eigvec[3];
		float radius;

		float id;
		ss >> id;

		ss >> pos[0];
		ss >> pos[1];
		ss >> pos[2];

		ss >> radius;

		ss >> eigval[0];
		ss >> eigval[1];
		ss >> eigval[2];

		ss >> eigvec[0];
		ss >> eigvec[1];
		ss >> eigvec[2];
		float3 e0 = make_float3(eigvec[0], eigvec[1], eigvec[2]);
		
		ss >> eigvec[0];
		ss >> eigvec[1];
		ss >> eigvec[2];
		float3 e1 = make_float3(eigvec[0], eigvec[1], eigvec[2]);

		ss >> eigvec[0];
		ss >> eigvec[1];
		ss >> eigvec[2];
		float3 e2 = make_float3(eigvec[0], eigvec[1], eigvec[2]);

		int nBin = cubemap_size * cubemap_size * 6;
		float *cubemap = new float[nBin];

		for (int i = 0; i < nBin; i++)	{
			ss >> cubemap[i];
		}
		
		float3 eigvec3 = make_float3(eigvec[0], eigvec[1], eigvec[2]);
		halos.push_back(new Halo(
			pos[0], pos[1], pos[2],
			radius,
			make_float3(eigval[0], eigval[1], eigval[2]),
			e0, e1, e2,
			cubemap, cubemap_size
			));
		std::getline(fin, line);
//		fin >> data;               //get next number from file
	}
	fin.close();       //close file
	//assert(!fin.fail());

}
