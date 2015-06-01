/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glextensions.h"
#include "glbuffers.h"
#include "gltrianglemesh.h"
#include "glSuperquadric.h"

VertexDescription P3T2N3Vertex::description[] = {
		{ VertexDescription::Position, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, position) / sizeof(float), 0, 0 },
		{ VertexDescription::TexCoord, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, texCoord) / sizeof(float), sizeof(QVector3D), 0 },
		{ VertexDescription::Normal, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, normal) / sizeof(float), sizeof(QVector3D) + sizeof(QVector3D), 0 },
		{ VertexDescription::Null, 0, 0, 0, 0 },
};


//VertexDescription P3T2N3Vertex::description[] = {
//	{ VertexDescription::Position, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, position) / sizeof(float), 0, 0 },
//	{ VertexDescription::TexCoord, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, texCoord) / sizeof(float), sizeof(QVector3D), 0 },
//	{ VertexDescription::Normal, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, normal) / sizeof(float), sizeof(QVector3D)+sizeof(QVector2D), 0 },
//
//	{ VertexDescription::Null, 0, 0, 0, 0 },
//};


//float lerp(float a, float b, float t)
//{
//	return a * (1.0f - t) + b * t;
//}

inline float SignedPow(float x, float a)
{
	int sign = x > 0 ? 1 : -1;
	return sign * pow(abs(x), a);
}

//TODO: verify the counts are correct
GLSuperquadric::GLSuperquadric(float3 e_val, float3 e_vec_0, float3 e_vec_1, float3 e_vec_2, float r, float scale, int n)
: GLTriangleMesh<P3T2N3Vertex, unsigned short>(2 * n * (n + 1), 12 * n * n)
{
	//scale = 0.2;
	int vidx = 0, iidx = 0;

	P3T2N3Vertex *vp = m_vb.lock();
	unsigned short *ip = m_ib.lock();

	if (!vp || !ip) {
		qWarning("GLRoundedBox::GLRoundedBox: Failed to lock vertex buffer and/or index buffer.");
		m_ib.unlock();
		m_vb.unlock();
		return;
	}
	//e_val.x = 0.45;
	//e_val.y = 0.44;
	//e_val.z = 0.11;
	float e_sum = e_val.x + e_val.y + e_val.z;
	float cl = (e_val.x - e_val.y) / e_sum;
	float cp = 2 * (e_val.y - e_val.z) / e_sum;
	float cs = 3 * e_val.z / e_sum;

	float a;// = pow(1 - cp, r);
	float b;// = pow(1 - cl, r);
	if (cl >= cp)	{
		a = pow(1 - cp, r);
		b = pow(1 - cl, r);
	}
	else {
		a = pow(1 - cl, r);
		b = pow(1 - cp, r);
	}
	//a = 1;
	//b = 0.5;

	float x, y, z;
	float theta, phi;
	int ni = n;
	int nj = 2 * n;

	//The inverse of rotation matrix is its transpose
	//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche53.html
	QMatrix4x4 trans(
		e_vec_0.x, e_vec_0.y, e_vec_0.z, 0,
		e_vec_1.x, e_vec_1.y, e_vec_1.z, 0,
		e_vec_2.x, e_vec_2.y, e_vec_2.z, 0,
		0, 0, 0, 1
		);
	//trans = trans.inverted();
	//trans.setToIdentity();
	trans = trans.transposed();
	//this mesh is not correct which can be seen as wireframe
	for (int i = 0; i <= ni; i++){
		phi = (float)i / ni * M_PI;
		for (int j = 0; j < nj; j++)	{
			theta = (float)j / nj * 2 * M_PI;
			if (cl >= cp)	{
				x = SignedPow(cos(phi), b);
				y = - SignedPow(sin(theta), a) * SignedPow(sin(phi), b);
				z = SignedPow(cos(theta), a) * SignedPow(sin(phi), b);
				y *= (cp+0.1);
				z *= (cp+0.1);
			}
			else{
				x = SignedPow(cos(theta), a) * SignedPow(sin(phi), b);
				y = SignedPow(sin(theta), a) * SignedPow(sin(phi), b);
				z = SignedPow(cos(phi), b);
				z *= (cl + 0.1);
			}
			QVector4D v = trans * QVector4D(x, y, z, 1);
			vp[vidx].position = QVector3D(v.x(), v.y(), v.z()) * scale;
			vp[vidx].normal = vp[vidx].position;// QVector3D(0.0f, 0.0f, 1.0f);// QVector3D(v.x(), v.y(), v.z()).normalized();
			vp[vidx].texCoord = vp[vidx].position;
			vidx++;

			if (i != ni)	{
				ip[iidx++] = i * nj + j;
				ip[iidx++] = (i + 1) * nj + j;
				ip[iidx++] = (i + 1) * nj + ((j + 1) % nj);

				ip[iidx++] = i * nj + j;
				ip[iidx++] = (i + 1) * nj + ((j + 1) % nj);
				ip[iidx++] = i * nj + ((j + 1) % nj);
			}
		}
	}

	m_ib.unlock();
	m_vb.unlock();
}

GLSuperquadric::~GLSuperquadric()
{
	//Need to delete textures here, presumably

}
