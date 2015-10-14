#ifndef MESH_READER_H
#define MESH_READER_H

#include <fstream>
#include <iostream>
#define _USE_MATH_DEFINES
#include "math.h"

struct MeshReader
{
	int TotalConnectedTriangles;
	//int TotalConnectedQuads;
	int numVerts;
	int TotalFaces;

	float* Faces_Triangles;
	float* Normals;
	unsigned int* indices;
	int numElements;

	void LoadPLY(const char* filename);

	void SphereMesh(float radius, unsigned int rings, unsigned int sectors);
};



#endif