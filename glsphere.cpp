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

#include "glsphere.h"


VertexDescription P3T2N3Vertex::description[] = {
	{ VertexDescription::Position, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, position) / sizeof(float), 0, 0 },
	{ VertexDescription::TexCoord, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, texCoord) / sizeof(float), sizeof(QVector3D), 0 },
	{ VertexDescription::Normal, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, normal) / sizeof(float), sizeof(QVector3D)+sizeof(QVector2D), 0 },

	{ VertexDescription::Null, 0, 0, 0, 0 },
};


//float lerp(float a, float b, float t)
//{
//	return a * (1.0f - t) + b * t;
//}


GLSphere::GLSphere(float r, float scale, int n)
: GLTriangleMesh<P3T2N3Vertex, unsigned short>((n + 1) * (n + 1) * 6, 36 * n * n)
{
	//scale = 0.25 * scale;
	int vidx = 0, iidx = 0;

	P3T2N3Vertex *vp = m_vb.lock();
	unsigned short *ip = m_ib.lock();

	if (!vp || !ip) {
		qWarning("GLRoundedBox::GLRoundedBox: Failed to lock vertex buffer and/or index buffer.");
		m_ib.unlock();
		m_vb.unlock();
		return;
	}

	float x, y, z;
	for (int f = 0; f < 6; f++)
	{
		for (int i = 0; i <= n; i++)	{
			float x = (float)i / n - 0.5;
			for (int j = 0; j <= n; j++)	{
				float y = (float)j / n - 0.5;
				int fcoord = f / 2;
				int fsign = pow(-1, f % 2);
				QVector3D v(1.0,1.0,1.0);
				//v.setX(0.5 * pow(-1, fsign));
				//v[0] = 0.5;// *pow(-1, fsign);
				if (0 == fcoord)	{
					v.setX(0.5 * fsign * scale);
					v.setY(x * fsign * scale);
					v.setZ(y * scale);
				}
				else if (1 == fcoord)	{
					v.setX(y * scale);
					v.setY(0.5 * fsign * scale);
					v.setZ(x * fsign * scale);
				}
				else{
					v.setX(x * fsign * scale);
					v.setY(y * scale);
					v.setZ(0.5 * fsign * scale);
				}
				//grid.push_back(v);
				vp[vidx].position = v.normalized();
				vp[vidx].normal = vp[vidx].position;
				vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
				vidx++;
				//int first = (nSub + 1) * j + i;
				if (i != n && j != n)	{
					ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j;
					ip[iidx++] = f * (n + 1) * (n + 1) + (i + 1) * (n + 1) + j;
					ip[iidx++] = f * (n + 1) * (n + 1) + (i + 1) * (n + 1) + j + 1;

					ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j;
					ip[iidx++] = f * (n + 1) * (n + 1) + (i + 1) * (n + 1) + j + 1;
					ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j + 1;

					//ip[iidx++] = f * (n + 1) * (n + 1) + (i - 1) * (n + 1) + j;
					//ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j + 1;
					//ip[iidx++] = f * (n + 1) * (n + 1) + (i - 1) * (n + 1) + j + 1;
					//faces.push_back(make_int3(first, first + nSub + 1, first + 1));// , first + nSub + 1));
					//faces.push_back(make_int3(first + 1, first + nSub + 1, first + nSub + 2));
				}
			}
			//		grid.push_back(row);
		}
	}
	m_ib.unlock();
	m_vb.unlock();
}

