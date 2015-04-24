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
	LoadParameters();
}

void DataManager::LoadParameters()
{
	QList<QString> filter = QStringList("*.par");
	QList<QFileInfo> files = QDir(":/res/vechist/").entryInfoList(filter, QDir::Files | QDir::Readable);

	foreach(QFileInfo fileInfo, files) {
		QFile file(fileInfo.absoluteFilePath());
		if (file.open(QIODevice::ReadOnly)) {
			while (!file.atEnd()) {
				QList<QByteArray> tokens = file.readLine().simplified().split(' ');
				QList<QByteArray>::const_iterator it = tokens.begin();
				if (it == tokens.end())
					continue;
				QByteArray type = *it;
				if (++it == tokens.end())
					continue;
				QByteArray name = *it;
				bool singleElement = (tokens.size() == 3); // type, name and one value
				char counter[10] = "000000000";
				int counterPos = 8; // position of last digit
				while (++it != tokens.end()) {
					if (type == "string") {
						valStrings[name.toStdString()] = it->toStdString();
					}

					//m_parameterNames << name;
					//if (!singleElement) {
					//    m_parameterNames.back() += "[";
					//    m_parameterNames.back() += counter + counterPos;
					//    m_parameterNames.back() += "]";
					//    int j = 8; // position of last digit
					//    ++counter[j];
					//    while (j > 0 && counter[j] > '9') {
					//        counter[j] = '0';
					//        ++counter[--j];
					//    }
					//    if (j < counterPos)
					//        counterPos = j;
					//}

					//if (type == "color") {
					//    layout->addWidget(new QLabel(m_parameterNames.back()));
					//    bool ok;
					//    ColorEdit *colorEdit = new ColorEdit(it->toUInt(&ok, 16), m_parameterNames.size() - 1);
					//    m_parameterEdits << colorEdit;
					//    layout->addWidget(colorEdit);
					//    connect(colorEdit, SIGNAL(colorChanged(QRgb,int)), this, SLOT(setColorParameter(QRgb,int)));
					//    ++row;
					//} else if (type == "float") {
					//    layout->addWidget(new QLabel(m_parameterNames.back()));
					//    bool ok;
					//    FloatEdit *floatEdit = new FloatEdit(it->toFloat(&ok), m_parameterNames.size() - 1);
					//    m_parameterEdits << floatEdit;
					//    layout->addWidget(floatEdit);
					//    connect(floatEdit, SIGNAL(valueChanged(float,int)), this, SLOT(setFloatParameter(float,int)));
					//    ++row;
					//}
				}
			}
			file.close();
		}
	}

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


string DataManager::GetStringVal(string name)
{
	return valStrings[name];
}

