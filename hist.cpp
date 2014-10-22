/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "hist.h"

#include <QGLWidget>
#include <QMatrix4x4>
#include <QVector3D>

#include <qmath.h>

#include "DataManager.h"
#include "HistMesh.h"
#include "GL\glu.h"
#include <vector_types.h>


static const qreal tee_height = 0.311126;
static const qreal cross_width = 0.25;
static const qreal bar_thickness = 0.113137;
static const qreal logo_depth = 0.10;
static const qreal pi_180 = M_PI / 180.0;

//https://github.com/joeld42/hexplanet/blob/master/src/hexplanet.cpp



//! [0]
struct Geometry
{
	QVector<GLushort> faces;
	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	void appendSmooth(const QVector3D &a, const QVector3D &n, int from);
	void appendFaceted(const QVector3D &a, const QVector3D &n);
	void finalize();
	void loadArrays() const;
};
//! [0]

//! [1]
class Patch
{
public:
	enum Smoothing { Faceted, Smooth };
	Patch(Geometry *);
	void setSmoothing(Smoothing s) { sm = s; }
	void translate(const QVector3D &t);
	void rotate(qreal deg, QVector3D axis);
	void draw() const;
	void addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n);
	void addQuad(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &d);

	GLushort start;
	GLushort count;
	GLushort initv;

	GLfloat faceColor[4];
	QMatrix4x4 mat;
	Smoothing sm;
	Geometry *geom;
};
//! [1]

static inline void qSetColor(float colorVec[], QColor c)
{
	colorVec[0] = c.redF();
	colorVec[1] = c.greenF();
	colorVec[2] = c.blueF();
	colorVec[3] = c.alphaF();
}

void Geometry::loadArrays() const
{
	glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
	glNormalPointer(GL_FLOAT, 0, normals.constData());
}

void Geometry::finalize()
{
	// TODO: add vertex buffer uploading here

	// Finish smoothing normals by ensuring accumulated normals are returned
	// to length 1.0.
	for (int i = 0; i < normals.count(); ++i)
		normals[i].normalize();
}

void Geometry::appendSmooth(const QVector3D &a, const QVector3D &n, int from)
{
	// Smooth normals are achieved by averaging the normals for faces meeting
	// at a point.  First find the point in geometry already generated
	// (working backwards, since most often the points shared are between faces
	// recently added).
	int v = vertices.count() - 1;
	for (; v >= from; --v)
	if (qFuzzyCompare(vertices[v], a))
		break;

	if (v < from) {
		// The vertex was not found so add it as a new one, and initialize
		// its corresponding normal
		v = vertices.count();
		vertices.append(a);
		normals.append(n);
	}
	else {
		// Vert found, accumulate normals into corresponding normal slot.
		// Must call finalize once finished accumulating normals
		normals[v] += n;
	}

	// In both cases (found or not) reference the vertex via its index
	faces.append(v);
}

void Geometry::appendFaceted(const QVector3D &a, const QVector3D &n)
{
	// Faceted normals are achieved by duplicating the vertex for every
	// normal, so that faces meeting at a vertex get a sharp edge.
	int v = vertices.count();
	vertices.append(a);
	normals.append(n);
	faces.append(v);
}

Patch::Patch(Geometry *g)
: start(g->faces.count())
, count(0)
, initv(g->vertices.count())
, sm(Patch::Smooth)
, geom(g)
{
	qSetColor(faceColor, QColor(Qt::darkGray));
}

void Patch::rotate(qreal deg, QVector3D axis)
{
	mat.rotate(deg, axis);
}

void Patch::translate(const QVector3D &t)
{
	mat.translate(t);
}

//! [2]
void Patch::draw() const
{
	glPushMatrix();
	glMultMatrixf(mat.constData());

	GLfloat tmpColor[4] = { 0.8f, 0.1f, 0.1f, 0.5f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tmpColor);
	//glColor4fv(tmpColor);
	//faceColor[3] = faceColor[3] * 0.5;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glBegin(GL_LINE_LOOP);
	GLUquadricObj* quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, 0.02, 32, 16);

	gluDeleteQuadric(quadric);
	//glEnd();

	GLfloat tmpColorTransp[4] = { faceColor[0], faceColor[1], faceColor[2], 0.5f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tmpColorTransp);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	const GLushort *indices = geom->faces.constData();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices + start);

	glPopMatrix();

}
//! [2]

void Patch::addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n)
{
	QVector3D norm = n.isNull() ? QVector3D::normal(a, b, c) : n;

	if (sm == Smooth) {
		geom->appendSmooth(a, norm, initv);
		geom->appendSmooth(b, norm, initv);
		geom->appendSmooth(c, norm, initv);
	}
	else {
		geom->appendFaceted(a, norm);
		geom->appendFaceted(b, norm);
		geom->appendFaceted(c, norm);
	}

	count += 3;
}

void Patch::addQuad(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &d)
{
	QVector3D norm = QVector3D::normal(a, b, c);

	if (sm == Smooth) {
		addTri(a, b, c, norm);
		addTri(a, c, d, norm);
	}
	else {
		// If faceted share the two common vertices
		addTri(a, b, c, norm);
		int k = geom->vertices.count();
		geom->appendSmooth(a, norm, k);
		geom->appendSmooth(c, norm, k);
		geom->appendFaceted(d, norm);
		count += 3;
	}
}

static inline QVector<QVector3D> extrude(const QVector<QVector3D> &vertices, qreal depth)
{
	QVector<QVector3D> extr = vertices;
	for (int v = 0; v < extr.count(); ++v)
		extr[v].setZ(extr[v].z() - depth);
	return extr;
}

class Rectoid
{
public:
	void translate(const QVector3D &t)
	{
		for (int i = 0; i < parts.count(); ++i)
			parts[i]->translate(t);
	}

	void rotate(qreal deg, QVector3D axis)
	{
		for (int i = 0; i < parts.count(); ++i)
			parts[i]->rotate(deg, axis);
	}

	// No special Rectoid destructor - the parts are fetched out of this member
	// variable, and destroyed by the new owner
	QList<Patch *> parts;
};

class Dice : public Rectoid
{
	Geometry *_g;
	QVector3D _center;
	QVector<QVector3D> _corners;
	void AddWedge(const int v0, const int v1, const int v2, qreal r);
public:
	Dice(Geometry *g, const QVector3D center);
	void SetCorners(std::vector<std::vector<double>> v);
	void BuildMesh(std::vector<std::vector<int>> vF);
};

void Dice::SetCorners(std::vector<std::vector<double>> v)
{
	for (int i = 0; i < v.size(); i++)	{
		_corners.push_back(QVector3D(v[i][0], v[i][1], v[i][2]));
	}
}

void Dice::BuildMesh(std::vector<std::vector<int>> vF)
{
	Patch *wedge = new Patch(_g);
	QVector3D nullVector3D;
	const float ss = 0.6;
	for (int i = 0; i < vF.size(); i++)	{
		wedge->addTri(_corners[vF[i][0]] * ss, _corners[vF[i][1]] * ss, _corners[vF[i][2]] * ss, nullVector3D);
	}
	parts << wedge;
}

Dice::Dice(Geometry *g, const QVector3D center)
{
	_g = g;
	_center = center;

}

void Dice::AddWedge(const int i0, const int i1, const int i2, qreal r)
{
	Patch *wedge = new Patch(_g);
	QVector3D nullVector3D;
	QVector3D c = (_corners[i0] + _corners[i1] + _corners[i2]) / 3;
	//QVector3D v0 = (c + _corners[i0]) * 0.5;
	//QVector3D v1 = (c + _corners[i1]) * 0.5;
	//QVector3D v2 = (c + _corners[i2]) * 0.5;
	const qreal t = 0.9;
	QVector3D v0 = c * t + _corners[i0] * (1 - t);
	QVector3D v1 = c * t + _corners[i1] * (1 - t);
	QVector3D v2 = c * t + _corners[i2] * (1 - t);

	wedge->addTri(_center + v2 * r, _center + v1 * r, _center + v0 * r, nullVector3D);
	wedge->addTri(_center, _center + v0 * r, _center + v1 * r, nullVector3D);
	wedge->addTri(_center, _center + v1 * r, _center + v2 * r, nullVector3D);
	wedge->addTri(_center, _center + v2 * r, _center + v0 * r, nullVector3D);
	parts << wedge;
}

class Glyph : public Rectoid
{
	Geometry *_g;
	QVector3D _center;
	QVector<QVector3D> _corners;
	qreal _scale;
	void AddWedge(const int i0, const int i1, const int i2, const int i3, qreal r);
public:
	Glyph(Geometry *g, const QVector3D center, const QVector<qreal> hist, const qreal scale);
};

void Glyph::AddWedge(const int i0, const int i1, const int i2, const int i3, qreal r)
{
	Patch *wedge = new Patch(_g);
	QVector3D nullVector3D;
	QVector3D c = (_corners[i0] + _corners[i1] + _corners[i2] + _corners[i3]) / 4;
	const qreal t = 0.9;
	QVector3D v0 = c * t + _corners[i0] * (1 - t);
	QVector3D v1 = c * t + _corners[i1] * (1 - t);
	QVector3D v2 = c * t + _corners[i2] * (1 - t);
	QVector3D v3 = c * t + _corners[i3] * (1 - t);

	wedge->addQuad(_center + v3 * r, _center + v2 * r, _center + v1 * r, _center + v0 * r);
	wedge->addTri(_center, _center + v0 * r, _center + v1 * r, nullVector3D);
	wedge->addTri(_center, _center + v1 * r, _center + v2 * r, nullVector3D);
	wedge->addTri(_center, _center + v2 * r, _center + v3 * r, nullVector3D);
	wedge->addTri(_center, _center + v3 * r, _center + v0 * r, nullVector3D);
	parts << wedge;
}

Glyph::Glyph(Geometry *g, const QVector3D center, const QVector<qreal> hist, const qreal scale)
{
	_g = g;
	_center = center;
	qreal lon[4] = { 45, 135, 225, 315 };
	qreal lat[2] = { -45, 45 };
	const qreal r = 0.3;
	/*for(int j = 0; j < 2; j++)
		for(int i = 0; i < 4; i++)
		corners.push_back(
		QVector3D(	qCos(lat[j] * pi_180) * qCos(lon[i] * pi_180),
		qCos(lat[j] * pi_180) * qSin(lon[i] * pi_180),
		qSin(lat[j] * pi_180))
		);*/
	_corners.push_back(QVector3D(-r, -r, -r));
	_corners.push_back(QVector3D(-r, r, -r));
	_corners.push_back(QVector3D(r, r, -r));
	_corners.push_back(QVector3D(r, -r, -r));
	_corners.push_back(QVector3D(-r, -r, r));
	_corners.push_back(QVector3D(-r, r, r));
	_corners.push_back(QVector3D(r, r, r));
	_corners.push_back(QVector3D(r, -r, r));

	AddWedge(0, 1, 5, 4, pow(hist[0], 0.333) * scale);
	AddWedge(1, 2, 6, 5, pow(hist[1], 0.333) * scale);
	AddWedge(2, 3, 7, 6, pow(hist[2], 0.333) * scale);
	AddWedge(3, 0, 4, 7, pow(hist[3], 0.333) * scale);
	AddWedge(4, 5, 6, 7, pow(hist[4], 0.333) * scale);
	AddWedge(3, 2, 1, 0, pow(hist[5], 0.333) * scale);
}

class RectPrism : public Rectoid
{
public:
	RectPrism(Geometry *g, qreal width, qreal height, qreal depth);
};

RectPrism::RectPrism(Geometry *g, qreal width, qreal height, qreal depth)
{
	enum { bl, br, tr, tl };
	Patch *fb = new Patch(g);
	fb->setSmoothing(Patch::Faceted);

	// front face
	QVector<QVector3D> r(4);
	r[br].setX(width);
	r[tr].setX(width);
	r[tr].setY(height);
	r[tl].setY(height);
	QVector3D adjToCenter(-width / 2.0, -height / 2.0, depth / 2.0);
	for (int i = 0; i < 4; ++i)
		r[i] += adjToCenter;
	fb->addQuad(r[bl], r[br], r[tr], r[tl]);

	// back face
	QVector<QVector3D> s = extrude(r, depth);
	fb->addQuad(s[tl], s[tr], s[br], s[bl]);

	// side faces
	Patch *sides = new Patch(g);
	sides->setSmoothing(Patch::Faceted);
	sides->addQuad(s[bl], s[br], r[br], r[bl]);
	sides->addQuad(s[br], s[tr], r[tr], r[br]);
	sides->addQuad(s[tr], s[tl], r[tl], r[tr]);
	sides->addQuad(s[tl], s[bl], r[bl], r[tl]);

	parts << fb << sides;
}

class RectTorus : public Rectoid
{
public:
	RectTorus(Geometry *g, qreal iRad, qreal oRad, qreal depth, int numSectors);
};

RectTorus::RectTorus(Geometry *g, qreal iRad, qreal oRad, qreal depth, int k)
{
	QVector<QVector3D> inside;
	QVector<QVector3D> outside;
	for (int i = 0; i < k; ++i) {
		qreal angle = (i * 2 * M_PI) / k;
		inside << QVector3D(iRad * qSin(angle), iRad * qCos(angle), depth / 2.0);
		outside << QVector3D(oRad * qSin(angle), oRad * qCos(angle), depth / 2.0);
	}
	inside << QVector3D(0.0, iRad, 0.0);
	outside << QVector3D(0.0, oRad, 0.0);
	QVector<QVector3D> in_back = extrude(inside, depth);
	QVector<QVector3D> out_back = extrude(outside, depth);

	// Create front, back and sides as separate patches so that smooth normals
	// are generated for the curving sides, but a faceted edge is created between
	// sides and front/back
	Patch *front = new Patch(g);
	for (int i = 0; i < k; ++i)
		front->addQuad(outside[i], inside[i],
		inside[(i + 1) % k], outside[(i + 1) % k]);
	Patch *back = new Patch(g);
	for (int i = 0; i < k; ++i)
		back->addQuad(in_back[i], out_back[i],
		out_back[(i + 1) % k], in_back[(i + 1) % k]);
	Patch *is = new Patch(g);
	for (int i = 0; i < k; ++i)
		is->addQuad(in_back[i], in_back[(i + 1) % k],
		inside[(i + 1) % k], inside[i]);
	Patch *os = new Patch(g);
	for (int i = 0; i < k; ++i)
		os->addQuad(out_back[(i + 1) % k], out_back[i],
		outside[i], outside[(i + 1) % k]);
	parts << front << back << is << os;
}

Hist::Hist(QObject *parent, char* filename, int divisions, qreal scale)
: QObject(parent)
, geom(new Geometry())
{
	dataManager = new DataManager();
	dataManager->LoadVec(filename);

	buildGeometry(divisions, scale);
}

Hist::~Hist()
{
	qDeleteAll(parts);
	delete geom;
}

void Hist::setColor(QColor c)
{
	for (int i = 0; i < parts.count(); ++i)
		qSetColor(parts[i]->faceColor, c);
}

//! [3]
void Hist::buildGeometry(int divisions, qreal scale)
{
	const qreal a = 0.2;
	//Glyph * glyph;
	Dice * glyph;
	//for(int i = 0 ; i < 1; i ++)
	//	for(int j = 0 ; j < 1; j ++)
	//		for(int k = 0 ; k < 1; k ++)
	//		{
	//	glyph = new Dice(geom, QVector3D( (i - 0.5) * 2 * a, (j - 0.5) * 2 * a, (k - 0.5) * 2 * a), values);
	//	glyph = new Glyph(geom, QVector3D( (i - 0.5) * 2 * a, (j - 0.5) * 2 * a, (k - 0.5) * 2 * a), values, 4);
	glyph = new Dice(geom, QVector3D(0, 0, 0));// , values);
//	glyph->SetCorners(dataManager->GetScaledVertexPos());
	std::vector<float3> datablock = dataManager->GetBlock(1, 1, 1, 2, 2, 2);
	HistMesh *histMesh = new HistMesh(5);
	std::vector<float3> vertices;
	std::vector<std::vector<int>> faces;
	histMesh->ComputeHistMesh(datablock, vertices, faces);

//	glyph->BuildMesh(dataManager->GetFaceVertices());
	//		glyph = new Glyph(geom, QVector3D( 0,0,0), values, 2);
	parts << glyph->parts;
	//}
	geom->finalize();
}
//! [3]

//! [4]
void Hist::draw() const
{
	geom->loadArrays();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	for (int i = 0; i < parts.count(); ++i)
		parts[i]->draw();
	glDisable(GL_BLEND);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
//! [4]
