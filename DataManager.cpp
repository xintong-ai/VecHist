#include "DataManager.h"
#include <stdio.h>
#include <iostream>
//#include "dstat.h"
#include <memory>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cuda/vector_functions.h>
#include <fstream>

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

int DataManager::GetCubemapSize()
{
	return cubemap_size;
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


