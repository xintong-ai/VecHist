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

#include "box.h"

//============================================================================//
//                                P3T2N3Vertex                                //
//============================================================================//

VertexDescription P3T2N3Vertex::description[] = {
	{ VertexDescription::Position, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, position) / sizeof(float), 0, 0 },
	{ VertexDescription::TexCoord, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, texCoord) / sizeof(float), sizeof(QVector3D), 0 },
	{ VertexDescription::Normal, GL_FLOAT, SIZE_OF_MEMBER(P3T2N3Vertex, normal) / sizeof(float), sizeof(QVector3D)+sizeof(QVector2D), 0 },

	{ VertexDescription::Null, 0, 0, 0, 0 },
};

//============================================================================//
//                                GLRoundedBox                                //
//============================================================================//

float lerp(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}

GLBox::GLBox(float r, float scale, int n)
: GLTriangleMesh<P3T2N3Vertex, unsigned short>(8, 36)
{
	int vidx = 0, iidx = 0;
	int vertexCountPerCorner = (n + 2) * (n + 3) / 2;

	P3T2N3Vertex *vp = m_vb.lock();
	unsigned short *ip = m_ib.lock();

	if (!vp || !ip) {
		qWarning("GLRoundedBox::GLRoundedBox: Failed to lock vertex buffer and/or index buffer.");
		m_ib.unlock();
		m_vb.unlock();
		return;
	}

	scale = scale * 0.5;

	//for (int i = 0; i < 8; i++)	{
	vp[vidx].position = scale * QVector3D(-1, -1, -1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(-1, -1, 1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(-1, 1, -1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(-1, 1, 1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(1, -1, -1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(1, -1, 1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(1, 1, -1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	vp[vidx].position = scale * QVector3D(1, 1, 1);
	vp[vidx].normal = vp[vidx].position.normalized();
	vp[vidx].texCoord = QVector2D(0.5f, 0.5f);
	vidx++;

	ip[iidx++] = 0;
	ip[iidx++] = 1;
	ip[iidx++] = 2;

	ip[iidx++] = 1;
	ip[iidx++] = 3;
	ip[iidx++] = 2;

	ip[iidx++] = 4;
	ip[iidx++] = 7;
	ip[iidx++] = 5;

	ip[iidx++] = 4;
	ip[iidx++] = 6;
	ip[iidx++] = 7;

	ip[iidx++] = 1;
	ip[iidx++] = 7;
	ip[iidx++] = 3;

	ip[iidx++] = 1;
	ip[iidx++] = 5;
	ip[iidx++] = 7;

	ip[iidx++] = 0;
	ip[iidx++] = 2;
	ip[iidx++] = 6;

	ip[iidx++] = 0;
	ip[iidx++] = 6;
	ip[iidx++] = 4;

	ip[iidx++] = 2;
	ip[iidx++] = 3;
	ip[iidx++] = 7;

	ip[iidx++] = 2;
	ip[iidx++] = 7;
	ip[iidx++] = 6;

	ip[iidx++] = 0;
	ip[iidx++] = 5;
	ip[iidx++] = 1;

	ip[iidx++] = 0;
	ip[iidx++] = 4;
	ip[iidx++] = 5;


	m_ib.unlock();
	m_vb.unlock();
}

