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

#include <QDebug>
#include "scene.h"
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qvector3d.h>
#include <QOpenGLFunctions>
#include <sstream>

//#include "3rdparty/fbm.h"
#include <memory>
//#include <driver_types.h>
//#include <driver_functions.h>
//#include <helper_cuda.h>
//#include <helper_math.h>

//#include <qopenglext.h>

/*
extern "C" void inputMask(void *h_volume, cudaExtent volumeSize);

extern "C" void setTextureFilterMode(bool bLinearFilter);
//extern "C" void initCuda(void *h_volume, cudaExtent volumeSize);
extern "C" void freeCudaBuffers();
extern "C" void render_kernel(dim3 gridSize, dim3 blockSize, uint *d_output, uint imageW, uint imageH);//,
//	float density, float brightness, float transferOffset, float transferScale);
//extern "C" void copyInvViewMatrix(float *invViewMatrix, size_t sizeofMatrix);
//extern "C" void copyInvProjMatrix(float *invProjMatrix, size_t sizeofMatrix);
extern "C" void copyInvProjMulViewMatrix(float *InvProjMulViewMatrix, size_t sizeofMatrix);
extern "C" void copyDataDim(int *dataDim, size_t size);
*/
//#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a) = (b); (b) = _tmp; }
//#define MAT(m,r,c) (m)[(c)*4+(r)]

//inline float3 operator*(const float3 &a, const float &b) {
//	return make_float3(a.x * b, a.y * b, a.z * b);
//}

//inline float3 operator+(const float3 &a, const float3 &b) {
//	return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
//}
//
//inline float3 operator-(const float3 &a, const float3 &b) {
//	return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
//}
//
//inline float3 cross(float3 a, float3 b)
//{
//	return make_float3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
//}
//
//inline float dot(float3 a, float3 b)
//{
//	return a.x * b.x + a.y * b.y + a.z * b.z;
//}
//
//

//void sort_points(float3 *points, unsigned point_count, const float4 &plane)
//{
//	if (point_count == 0) return;
//
//	const float3 plane_normal = make_float3(plane.x, plane.y, plane.z);
//	const float3 origin = points[0];
//
//	std::sort(points, points + point_count, [&](const float3 &lhs, const float3 &rhs) -> bool {
//		float3 v;
//		v = cross((lhs - origin), (rhs - origin));
//		return dot(v, plane_normal) > 0;
//	});
//}
//
//bool ray_to_plane(const float3 &RayOrig, const float3 &RayDir, const float4 &Plane, float *OutT, float *OutVD)
//{
//	*OutVD = Plane.x * RayDir.x + Plane.y * RayDir.y + Plane.z * RayDir.z;
//	if (*OutVD == 0.0f)
//		return false;
//	*OutT = -(Plane.x * RayOrig.x + Plane.y * RayOrig.y + Plane.z * RayOrig.z + Plane.w) / *OutVD;
//	return true;
//}
//
//void calc_plane_aabb_intersection_points(const float4 &plane,
//	const float3 &aabb_min, const float3 &aabb_max,
//	float3 *out_points, unsigned &out_point_count)
//{
//	out_point_count = 0;
//	float vd, t;
//
//	// Test edges along X axis, pointing right.
//	float3 dir = make_float3(aabb_max.x - aabb_min.x, 0.f, 0.f);
//	float3 orig = aabb_min;
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_min.x, aabb_max.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_min.x, aabb_min.y, aabb_max.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_min.x, aabb_max.y, aabb_max.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//
//	// Test edges along Y axis, pointing up.
//	dir = make_float3(0.f, aabb_max.y - aabb_min.y, 0.f);
//	orig = make_float3(aabb_min.x, aabb_min.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_max.x, aabb_min.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_min.x, aabb_min.y, aabb_max.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_max.x, aabb_min.y, aabb_max.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//
//	// Test edges along Z axis, pointing forward.
//	dir = make_float3(0.f, 0.f, aabb_max.z - aabb_min.z);
//	orig = make_float3(aabb_min.x, aabb_min.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_max.x, aabb_min.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_min.x, aabb_max.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//	orig = make_float3(aabb_max.x, aabb_max.y, aabb_min.z);
//	if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
//		out_points[out_point_count++] = orig + dir * t;
//}
//
//typedef struct {
//	double r, g, b;
//} COLOUR;
//
//COLOUR GetColour(double v, double vmin, double vmax)
//{
//	COLOUR c = { 1.0, 1.0, 1.0 }; // white
//	double dv;
//
//	if (v < vmin)
//		v = vmin;
//	if (v > vmax)
//		v = vmax;
//	dv = vmax - vmin;
//
//	if (v < (vmin + 0.25 * dv)) {
//		c.r = 0;
//		c.g = 4 * (v - vmin) / dv;
//	}
//	else if (v < (vmin + 0.5 * dv)) {
//		c.r = 0;
//		c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
//	}
//	else if (v < (vmin + 0.75 * dv)) {
//		c.r = 4 * (v - vmin - 0.5 * dv) / dv;
//		c.b = 0;
//	}
//	else {
//		c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
//		c.b = 0;
//	}
//
//	return(c);
//}

void checkGLErrors(const QString& prefix)
{
    switch (glGetError()) {
    case GL_NO_ERROR:
        //qDebug() << prefix << tr("No error.");
        break;
    case GL_INVALID_ENUM:
        qDebug() << prefix << QObject::tr("Invalid enum.");
        break;
    case GL_INVALID_VALUE:
        qDebug() << prefix << QObject::tr("Invalid value.");
        break;
    case GL_INVALID_OPERATION:
        qDebug() << prefix << QObject::tr("Invalid operation.");
        break;
    case GL_STACK_OVERFLOW:
        qDebug() << prefix << QObject::tr("Stack overflow.");
        break;
    case GL_STACK_UNDERFLOW:
        qDebug() << prefix << QObject::tr("Stack underflow.");
        break;
    case GL_OUT_OF_MEMORY:
        qDebug() << prefix << QObject::tr("Out of memory.");
        break;
    default:
        qDebug() << prefix << QObject::tr("Unknown error.");
        break;
    }
}

//void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric)
//{
//	float vx = x2 - x1;
//	float vy = y2 - y1;
//	float vz = z2 - z1;
//
//	//handle the degenerate case of z1 == z2 with an approximation
//	if (vz == 0)
//		vz = .0001;
//
//	float v = sqrt(vx*vx + vy*vy + vz*vz);
//	float ax = 57.2957795*acos(vz / v);
//	if (vz < 0.0)
//		ax = -ax;
//	float rx = -vy*vz;
//	float ry = vx*vz;
//	glPushMatrix();
//
//	//draw the cylinder body
//	glTranslatef(x1, y1, z1);
//	glRotatef(ax, rx, ry, 0.0);
//	gluQuadricOrientation(quadric, GLU_OUTSIDE);
//	gluCylinder(quadric, radius, radius, v, subdivisions, 1);
//
//	////draw the first cap
//	//gluQuadricOrientation(quadric, GLU_INSIDE);
//	//gluDisk(quadric, 0.0, radius, subdivisions, 1);
//	//glTranslatef(0, 0, v);
//
//	////draw the second cap
//	//gluQuadricOrientation(quadric, GLU_OUTSIDE);
//	//gluDisk(quadric, 0.0, radius, subdivisions, 1);
//	glPopMatrix();
//}
//
//void renderCylinder_convenient(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions)
//{
//	//the same quadric can be re-used for drawing many cylinders
//	GLUquadricObj *quadric = gluNewQuadric();
//	gluQuadricNormals(quadric, GLU_SMOOTH);
//	renderCylinder(x1, y1, z1, x2, y2, z2, radius, subdivisions, quadric);
//	gluDeleteQuadric(quadric);
//}
//
//void renderCone(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric)
//{
//	float vx = x2 - x1;
//	float vy = y2 - y1;
//	float vz = z2 - z1;
//
//	//handle the degenerate case of z1 == z2 with an approximation
//	if (vz == 0)
//		vz = .0001;
//
//	float v = sqrt(vx*vx + vy*vy + vz*vz);
//	float ax = 57.2957795*acos(vz / v);
//	if (vz < 0.0)
//		ax = -ax;
//	float rx = -vy*vz;
//	float ry = vx*vz;
//	glPushMatrix();
//
//	//draw the cylinder body
//	glTranslatef(x1, y1, z1);
//	glRotatef(ax, rx, ry, 0.0);
//	gluQuadricOrientation(quadric, GLU_OUTSIDE);
//	gluCylinder(quadric, radius, radius * 0.1, v, subdivisions, 1);
//
//	////draw the first cap
//	//gluQuadricOrientation(quadric, GLU_INSIDE);
//	//gluDisk(quadric, 0.0, radius, subdivisions, 1);
//	//glTranslatef(0, 0, v);
//
//	////draw the second cap
//	//gluQuadricOrientation(quadric, GLU_OUTSIDE);
//	//gluDisk(quadric, 0.0, radius, subdivisions, 1);
//	glPopMatrix();
//}
//
//void renderCone_convenient(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions)
//{
//	//the same quadric can be re-used for drawing many cylinders
//	GLUquadricObj *quadric = gluNewQuadric();
//	gluQuadricNormals(quadric, GLU_SMOOTH);
//	renderCone(x1, y1, z1, x2, y2, z2, radius, subdivisions, quadric);
//	gluDeleteQuadric(quadric);
//}

//============================================================================//
//                           TwoSidedGraphicsWidget                           //
//============================================================================//

TwoSidedGraphicsWidget::TwoSidedGraphicsWidget(QGraphicsScene *scene)
    : QObject(scene)
    , m_current(0)
    , m_angle(0)
    , m_delta(0)
{
    for (int i = 0; i < 3; ++i)
        m_proxyWidgets[i] = 0;
}

void TwoSidedGraphicsWidget::setWidget(int index, QWidget *widget)
{
    if (index < 0 || index >= 3)
    {
        qWarning("TwoSidedGraphicsWidget::setWidget: Index out of bounds, index == %d", index);
        return;
    }
	if (1 == application || 2 == application)	{
		GraphicsWidget *proxy = new GraphicsWidget;
		proxy->setWidget(widget);

		if (m_proxyWidgets[index])
			delete m_proxyWidgets[index];
		m_proxyWidgets[index] = proxy;

		proxy->setCacheMode(QGraphicsItem::ItemCoordinateCache);
		proxy->setZValue(1e30); // Make sure the dialog is drawn on top of all other (OpenGL) items

		//if (index != m_current)
		//    proxy->setVisible(false);
		proxy->setVisible(true);

		qobject_cast<QGraphicsScene *>(parent())->addItem(proxy);
	}
}

QWidget *TwoSidedGraphicsWidget::widget(int index)
{
    if (index < 0 || index >= 3)
    {
        qWarning("TwoSidedGraphicsWidget::widget: Index out of bounds, index == %d", index);
        return 0;
    }
    return m_proxyWidgets[index]->widget();
}

void TwoSidedGraphicsWidget::flip()
{
    m_delta = (m_current == 0 ? 9 : -9);
    animateFlip();
}

void TwoSidedGraphicsWidget::animateFlip()
{
    m_angle += m_delta;
    if (m_angle == 90) {
        int old = m_current;
        m_current ^= 1;
        m_proxyWidgets[old]->setVisible(false);
        m_proxyWidgets[m_current]->setVisible(true);
        m_proxyWidgets[m_current]->setGeometry(m_proxyWidgets[old]->geometry());
    }

    QRectF r = m_proxyWidgets[m_current]->boundingRect();
    m_proxyWidgets[m_current]->setTransform(QTransform()
        .translate(r.width() / 2, r.height() / 2)
        .rotate(m_angle - 180 * m_current, Qt::YAxis)
        .translate(-r.width() / 2, -r.height() / 2));

    if ((m_current == 0 && m_angle > 0) || (m_current == 1 && m_angle < 180))
        QTimer::singleShot(25, this, SLOT(animateFlip()));
}

QVariant GraphicsWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QRectF rect = boundingRect();
        QPointF pos = value.toPointF();
        QRectF sceneRect = scene()->sceneRect();
        if (pos.x() + rect.left() < sceneRect.left())
            pos.setX(sceneRect.left() - rect.left());
        else if (pos.x() + rect.right() >= sceneRect.right())
            pos.setX(sceneRect.right() - rect.right());
        if (pos.y() + rect.top() < sceneRect.top())
            pos.setY(sceneRect.top() - rect.top());
        else if (pos.y() + rect.bottom() >= sceneRect.bottom())
            pos.setY(sceneRect.bottom() - rect.bottom());
        return pos;
    }
    return QGraphicsProxyWidget::itemChange(change, value);
}

void GraphicsWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    setCacheMode(QGraphicsItem::NoCache);
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
    QGraphicsProxyWidget::resizeEvent(event);
}

void GraphicsWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    QGraphicsProxyWidget::paint(painter, option, widget);
    //painter->setRenderHint(QPainter::Antialiasing, true);
}

//============================================================================//
//                             RenderOptionsDialog                            //
//============================================================================//

RenderOptionsDialog::RenderOptionsDialog()
    : QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint)
{
    setWindowOpacity(0.75);
    setWindowTitle(tr("Options"));
    QGridLayout *layout = new QGridLayout;
    setLayout(layout);
    layout->setColumnStretch(1, 1);

    int row = 0;


    layout->addWidget(new QLabel(tr("Shader:")));
    m_shaderCombo = new QComboBox;
    connect(m_shaderCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(shaderChanged(int)));
    layout->addWidget(m_shaderCombo);
    ++row;

	layout->addWidget(new QLabel(tr("start x:")));
	layout->addWidget(new QLabel(tr("size x:")));
	++row;
	m_blockLoc << new QLineEdit();
	layout->addWidget(m_blockLoc.back());
	m_blockSize << new QLineEdit();
	layout->addWidget(m_blockSize.back());
	++row;

	layout->addWidget(new QLabel(tr("start y:")));
	layout->addWidget(new QLabel(tr("size y:")));
	++row;
	m_blockLoc << new QLineEdit();
	layout->addWidget(m_blockLoc.back());
	m_blockSize << new QLineEdit();
	layout->addWidget(m_blockSize.back());
	++row;

	layout->addWidget(new QLabel(tr("start z:")));
	layout->addWidget(new QLabel(tr("size z:")));
	++row;
	m_blockLoc << new QLineEdit();
	layout->addWidget(m_blockLoc.back());
	m_blockSize << new QLineEdit();
	layout->addWidget(m_blockSize.back());
	++row;

	m_updateButton = new QPushButton("Update Block");
	layout->addWidget(m_updateButton);

	m_query = new QPushButton("Do Query");
	layout->addWidget(m_query);

	m_segmentation = new QPushButton("Segmentation");
	layout->addWidget(m_segmentation);

	++row;

	connect(m_updateButton, SIGNAL(clicked()), this, SLOT(updateBlock()));
	connect(m_query, SIGNAL(clicked()), this, SLOT(doQuery()));
	connect(m_segmentation, SIGNAL(clicked()), this, SLOT(doSegmentation()));

    layout->setRowStretch(row, 1);
}

void RenderOptionsDialog::doQuery()
{
	emit queryChanged(0, 10, 10);
}

void RenderOptionsDialog::doSegmentation()
{
	emit segmentationRequested();
}

void RenderOptionsDialog::updateBlock()
{
	emit blockChanged(
		m_blockLoc[0]->text().toInt(), 
		m_blockLoc[1]->text().toInt(), 
		m_blockLoc[2]->text().toInt(),
		m_blockSize[0]->text().toInt(),
		m_blockSize[1]->text().toInt(),
		m_blockSize[2]->text().toInt()
		);
}
//
//int RenderOptionsDialog::addTexture(const QString &name)
//{
//    m_textureCombo->addItem(name);
//    return m_textureCombo->count() - 1;
//}

int RenderOptionsDialog::addShader(const QString &name)
{
    m_shaderCombo->addItem(name);
    return m_shaderCombo->count() - 1;
}

void RenderOptionsDialog::emitParameterChanged()
{
    foreach (ParameterEdit *edit, m_parameterEdits)
        edit->emitChange();
}

void RenderOptionsDialog::setColorParameter(QRgb color, int id)
{
    emit colorParameterChanged(m_parameterNames[id], color);
}

void RenderOptionsDialog::setFloatParameter(float value, int id)
{
    emit floatParameterChanged(m_parameterNames[id], value);
}

void RenderOptionsDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit doubleClicked();
}

//============================================================================//
//                                    Scene                                   //
//============================================================================//

const static char environmentShaderText[] =
"uniform samplerCube env;"
"void main() {"
//"float a = textureCube(env, gl_TexCoord[1].xyz).x * 0.1;"
//"gl_FragColor =  textureCube(env, gl_TexCoord[1].xyz);"//vec4(a, 0, 0, 1.0f);"//vec4(0.0,1.0,0.0,1.0) * 1000;"////
"gl_FragColor =  vec4(0.5f,0.5f,0.5f,1.0f);"
		"}";

inline int iDivUp(int a, int b)
{
	return (a % b != 0) ? (a / b + 1) : (a / b);
}

void Scene::UpdateTexture()
{
	blockTex.clear();

	leafNodes = dataManager->GetAllNode();

	for (auto nd : leafNodes)	{
		GLTextureCube *texCube = new GLTextureCube(qMin(1024, m_maxTextureSize), 1);
		texCube->load(nd->GetCubemap(), dataManager->GetCubemapSize());
		glColor3d(1.0, 1.0, 1.0);
		blockTex << texCube;
	}
}


Scene::Scene(int width, int height, int maxTextureSize)
    : m_distExp(600)
    , m_frame(0)
    , m_maxTextureSize(maxTextureSize)
   // , m_currentShader(0)
    //, m_currentTexture(0)
    , m_dynamicCubemap(false)
    //, m_updateAllCubemaps(true)
    //, m_vecWidget(0)
    , m_vertexShader(0)
    //, m_environmentShader(0)
    //, m_environmentProgram(0)
{
//	if (dataManager->GetStringVal("datatype").compare("flow") == 0)
	if (1 == application)
		dataManager = new DataMgrVect();
	else //if (dataManager->GetStringVal("datatype").compare("cosmology") == 0)
		dataManager = new DataMgrCosm();

	//Read the starting time step value from the .par file, and convert it to an integer
	string startTimeStepStr = dataManager->GetStringVal("starttimestep");
	istringstream iss(startTimeStepStr);
	int startTimeStepValue = 0;
	iss >> startTimeStepValue;
	dataManager->setStartTimeStep(startTimeStepValue);
	dataManager->setCurrentTimeStep(startTimeStepValue);

	pbo = GLuint(0);
	tex = 0;
	//$$$
	
	//dataManager->LoadVec("D:/data/sample/test2.vec");
	//dataManager->LoadVec("D:/data/sample/sample_two_halves.vec");
		//dataManager->LoadVec("D:/data/sample/test1.vec");
	//dataManager->LoadVec("C:/Users/tong.tong-idea/SkyDrive/share/15plume3d430.vec");
    //dataManager->LoadVec("D:/data/plume/15plume3d421.vec");
    ///////dataManager->LoadVec("/media/User/data/plume/15plume3d421.vec");
   

    //dataManager->LoadVec("C:\\Users\\datahead8888\\Documents\\sciVis\\data\\15plume3d421.vec");
	//dataManager->LoadVec("C:\\Users\\datahead8888\\Documents\\sciVis\\data\\15plume3d430.vec");
	//dataManager->LoadVec("C:\\Users\\datahead8888\\Documents\\sciVis\\data\\UVWf01.vec");
	

	//TO DO: Make this work:
	//dataManager->LoadVec(dataManager->GetFilename("vectorfield").c_str());
	dataManager->LoadData();
	UpdateTexture();

	//dataManager->LoadVec("D:/data/nek/nek.d_4.vec");
	
	//dataManager->LoadVec("D:/data/brain_dti/vector-field.vec");
	//dataManager->LoadVec("D:/data/isabel/UVWf01.vec");
	//dataManager->LoadVec("D:/data/nek/nek.d_4.vec");
	//dataManager->LoadVec("D:/data/tornado/1.vec");
	//dataManager->LoadVec("D:/data/saddle/saddle.vec");

	m_width = width;
	m_height = height;
	//blockSize.x = 16;
	//blockSize.y = 16;
	//gridSize = dim3(iDivUp(width, blockSize.x), iDivUp(height, blockSize.y));


    setSceneRect(0, 0, width, height);

    m_trackBalls[0] = TrackBall(0.05f, QVector3D(0, 1, 0), TrackBall::Sphere);
    //m_trackBalls[1] = TrackBall(0.005f, QVector3D(0, 0, 1), TrackBall::Sphere);
    //m_trackBalls[2] = TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::Plane);

    m_renderOptions = new RenderOptionsDialog();
    m_renderOptions->move(20, 120);
    m_renderOptions->resize(m_renderOptions->sizeHint());

	if (1 == application) {
		m_graphWidget = new GraphWidget();
		m_graphWidget->move(60, 120);
		m_graphWidget->resize(m_graphWidget->sizeHint());

		m_graphWidget->getTreeStats((NodeBi*)dataManager->getRootNode(), 0, 0);
		m_graphWidget->buildGraphFromTree((NodeBi*)dataManager->getRootNode());
	}
	else {
		m_graphWidget = new GraphWidget();
		m_graphWidget->move(60, 120);
		m_graphWidget->resize(m_graphWidget->sizeHint());

		

		QGraphicsView * view = new QGraphicsView();
		m_jsonView = new QJsonView(view, dataManager, this);

		scrollArea = new QScrollArea;
		scrollArea->setWidget(view);

		scrollArea->move(20, 120);
		scrollArea->resize(900, 800);

		view->resize(1000, 1000);

		//m_jsonView->move(60, 120);
		//m_jsonView->resize(m_jsonView->sizeHint());
		m_jsonView->resize(1000, 1000);
		

		m_listWidget = new QListWidget;
		m_listWidget->resize(200, 600);
		m_listWidget->move(20, 20);

		DataMgrCosm * cosmPtr = (DataMgrCosm *)dataManager;
		//vector<MergeTree *> forest = cosmPtr->getForest();

		//vector<MergeTree *> levelXForest = cosmPtr->getNodesAtGivenTimestepFromForest(16);
		levelXForest = cosmPtr->getNodesAtGivenTimestepFromForest(dataManager->getCurrentTimeStep());

		vector<MergeNode *>::iterator it;
		int i = 0;
		for (it = levelXForest.begin(); it != levelXForest.end(); it++) {
			m_listWidget->insertItem(i, QString::number((*it)->haloId));
			i++;
		}
		cout << endl;
		
		//Example from http://codereview.stackexchange.com/questions/11849/qjsonview-a-qwidget-based-json-explorer-for-qt
		//QString data = ((DataMgrCosm * ) dataManager)->getMergeTreeJSon(0);
		QString data = ((DataMgrCosm *)dataManager)->buildJsonFromTree(levelXForest[0], 0);

		m_jsonView->setJsonValue(data);

		//cout << "Data contents: " << endl;
		//cout << data.toStdString() << endl;

		
		

	}

	int nx, ny, nz;
	dataManager->GetVolumeSize(nx, ny, nz);
	m_renderOptions->setBlock(0, 0, 0, nx, ny, nz);

	//ShowGpuMemInfo();
	//m_vec3DTex = new GLTexture3D(nx, ny, nz);
	//m_vec3DTex->load(nx, ny, nz, dataManager->GetVecDataXFirst());
	cout << "Loading 3D texture..." << endl;
	//ShowGpuMemInfo();

	//initPixelBuffer();
	
    //connect(m_renderOptions, SIGNAL(colorParameterChanged(QString,QRgb)), this, SLOT(setColorParameter(QString,QRgb)));
    connect(m_renderOptions, SIGNAL(floatParameterChanged(QString,float)), this, SLOT(setFloatParameter(QString,float)));
    //connect(m_renderOptions, SIGNAL(shaderChanged(int)), this, SLOT(setShader(int)));
	connect(m_renderOptions, SIGNAL(blockChanged(int, int, int, int, int, int)), 
		this, SLOT(UpdateBlock(int, int, int, int, int, int)));
    connect(m_renderOptions, SIGNAL(queryChanged(int, int, int)), this, SLOT(UpdateQuery(int, int, int)));
	connect(m_renderOptions, SIGNAL(segmentationRequested()), this, SLOT(Segmentation()));
	connect(m_listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(dropBoxSelection()));

    TwoSidedGraphicsWidget *twoSided = new TwoSidedGraphicsWidget(this);
	//QDockWidget *dock = new QDockWidget(QString(tr("Parameters")), this);
    //twoSided->setWidget(0, m_renderOptions);
	//twoSided->setWidget(0, m_graphWidget);
	//twoSided->setWidget(0, view);
	twoSided->setWidget(0, scrollArea);
	twoSided->setWidget(1, m_renderOptions);
	twoSided->setWidget(2, m_listWidget);
	//twoSided->setWidget(1, m_graphWidget);

    initGL();
	//TODO: when width and height changes, this function should be called again
	//InitPicking(m_width, m_height);


    m_timer = new QTimer(this);
    m_timer->setInterval(20);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start();

    m_time.start();

	//for (int i = 0; i < 500; i++)	{
	//	COLOUR tmp = GetColour(i, 0, 500);
	//	colorMap.push_back(make_float3(tmp.r, tmp.g, tmp.b));
	//}
}

//Respond to selection events for the list box for tree selection for the forest
void Scene::dropBoxSelection()
{
	int selectedId = m_listWidget->row(m_listWidget->currentItem());
	cout << "Selected id: " << selectedId << endl;

	//QString data = ((DataMgrCosm *)dataManager)->getMergeTreeJSon(selectedId);
	QString data = ((DataMgrCosm *)dataManager)->buildJsonFromTree(levelXForest[selectedId], 0);
	m_jsonView->setJsonValue(data);

	
}

Scene::~Scene()
{
	if (m_vecWidget)
        delete m_vecWidget;
    //foreach (GLTexture *texture, m_textures)
    //    if (texture) delete texture;
    for (auto program : m_programs)
		if (program.second) delete program.second;
    if (m_vertexShader)
        delete m_vertexShader;
    foreach (QGLShader *shader, m_fragmentShaders)
        if (shader) delete shader;
    //foreach (GLRenderTargetCube *rt, m_cubemaps)
    //    if (rt) delete rt;
	//delete m_vec3DTex;

	delete m_renderOptions;
	delete m_graphWidget;
	//delete m_jsonView;

//	cudaDeviceReset();
//	cleanup();
}

void Scene::initGL()
{
	initializeGLFunctions();
	m_vecWidget = new GLSphere(1.0f, 0.5f, 10);
	//m_superWidget = new GLSuperquadric(make_float3(0.8, 0.45, 0.4), make_float3(1, 0, 0), 3, 0.5, 16);

    m_vertexShader = new QGLShader(QGLShader::Vertex);
    m_vertexShader->compileSourceFile(QLatin1String(":/res/vechist/basic.vsh"));

	m_environment = new GLTextureCube(qMin(1024, m_maxTextureSize), 1);

    QStringList filter;
    QList<QFileInfo> files;

    // Load all .fsh files as fragment shaders
   // "distribution" = 0;
    filter = QStringList("*.fsh");
    files = QDir(":/res/vechist/").entryInfoList(filter, QDir::Files | QDir::Readable);
    foreach (QFileInfo file, files) {
        QGLShaderProgram *program = new QGLShaderProgram;
        QGLShader* shader = new QGLShader(QGLShader::Fragment);
		//cout << "Loading shader: " << &file.absoluteFilePath();
		//cout << "Loading shader: "<< file.absoluteFilePath().toStdString();
		QString myString = file.absoluteFilePath();
		//cout << myString.toStdString() << endl;
		shader->compileSourceFile(file.absoluteFilePath());
        // The program does not take ownership over the shaders, so store them in a vector so they can be deleted afterwards.
        program->addShader(m_vertexShader);
        program->addShader(shader);
        if (!program->link()) {
            qWarning("Failed to compile and link shader program");
            qWarning("Vertex shader log:");
            qWarning() << m_vertexShader->log();
            qWarning() << "Fragment shader log ( file =" << file.absoluteFilePath() << "):";
            qWarning() << shader->log();
            qWarning("Shader program log:");
            qWarning() << program->log();

            delete shader;
            delete program;
            continue;
        }

        m_fragmentShaders << shader;
		string filename = file.fileName().toStdString();
		m_programs[filename.substr(0, filename.find(".", 0))] = program;
        m_renderOptions->addShader(file.baseName());

        //program->bind();
        ////m_cubemaps << ((program->uniformLocation("env") != -1) ? new GLRenderTargetCube(qMin(256, m_maxTextureSize)) : 0);
        //program->release();
    }

    //if (m_programs.size() == 0)
    //    m_programs << new QGLShaderProgram;

    m_renderOptions->emitParameterChanged();
}

static void loadMatrix(const QMatrix4x4& m)
{
    // static to prevent glLoadMatrixf to fail on certain drivers
    static GLfloat mat[16];
    const float *data = m.constData();
    for (int index = 0; index < 16; ++index)
        mat[index] = data[index];
    glLoadMatrixf(mat);
}


void Scene::renderBBox(const QMatrix4x4 &view)
{
	//int nx, ny, nz;
	//dataManager->GetVolumeSize(nx, ny, nz);

	int nx, ny, nz;
	dataManager->GetVolumeSize(nx, ny, nz);
	//nx -= 1;
	//ny -= 1;
	//nz -= 1;
	int bx = nx - 1, by = ny - 1, bz = nz - 1;
	
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(bx, 0, 0);

	glVertex3f(bx, 0, 0);
	glVertex3f(bx, by, 0);

	glVertex3f(bx, by, 0);
	glVertex3f(0, by, 0);

	glVertex3f(0, by, 0);
	glVertex3f(0, 0, 0);

	//////
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, bz);

	glVertex3f(bx, 0, 0);
	glVertex3f(bx, 0, bz);

	glVertex3f(bx, by, 0);
	glVertex3f(bx, by, bz);

	glVertex3f(0, by, 0);
	glVertex3f(0, by, bz);

	//////
	glVertex3f(0, 0, bz);
	glVertex3f(bx, 0, bz);

	glVertex3f(bx, 0, bz);
	glVertex3f(bx, by, bz);

	glVertex3f(bx, by, bz);
	glVertex3f(0, by, bz);

	glVertex3f(0, by, bz);
	glVertex3f(0, 0, bz);

	glEnd();
}

inline void RenderBox(const QMatrix4x4 &view, int sx, int sy, int sz, int nx, int ny, int nz)
{
	glBegin(GL_LINES);

	
	//////Front quad///////
	//Bottom line
	glVertex3f(sx, sy, sz);
	glVertex3f(sx + nx, sy, sz);

	//Top line
	glVertex3f(sx, sy + ny, sz);
	glVertex3f(sx + nx, sy + ny, sz);

	//Left line
	glVertex3f(sx, sy, sz);
	glVertex3f(sx, sy + ny, sz);

	//Right line
	glVertex3f(sx + nx, sy, sz);
	glVertex3f(sx + nx, sy + ny, sz);


	/////Back quad///////
	//Bottom line (from same persepctive as front quad)
	glVertex3f(sx, sy, sz + nz);
	glVertex3f(sx + nx, sy, sz + nz);

	//Top line
	glVertex3f(sx, sy + ny, sz + nz);
	glVertex3f(sx + nx, sy + ny, sz + nz);

	//Left line
	glVertex3f(sx, sy, sz + nz);
	glVertex3f(sx, sy + ny, sz + nz);

	//Right line
	glVertex3f(sx + nx, sy, sz + nz);
	glVertex3f(sx + nx, sy + ny, sz + nz);

	/////////Left quad/////
	//Bottom line
	glVertex3f(sx, sy, sz);
	glVertex3f(sx, sy, sz + nz);

	//Top line
	glVertex3f(sx, sy + ny, sz);
	glVertex3f(sx, sy + ny, sz + nz);

	/////////Right quad/////
	//Bottom line
	glVertex3f(sx + nx, sy, sz);
	glVertex3f(sx + nx, sy, sz + nz);

	//Top line
	glVertex3f(sx + nx, sy + ny, sz);
	glVertex3f(sx + nx, sy + ny, sz + nz);

	glEnd();
}

void Scene::setSelectionBoxPosition(int x, int y, int z)
{
	selectionX = x;
	selectionY = y;
	selectionZ = z;

}

void Scene::setSelectionBoxWidth(int width)
{
	selectionBoxWidth = width;
}


void Scene::renderQCube(const QMatrix4x4 &view)
{

	int x, y, z;
	int nx, ny, nz;
	dataManager->GetQCube(x, y, z, nx, ny, nz);

	glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex3f(x, y, z);
	glVertex3f(x + nx, y, z);

	glVertex3f(x + nx, y, z);
	glVertex3f(x + nx, y + ny, z);

	glVertex3f(x + nx, y + ny, z);
	glVertex3f(x, y + ny, z);

	glVertex3f(x, y + ny, z);
	glVertex3f(x, y, z);

	//////
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + nz);

	glVertex3f(x + nx, y, z);
	glVertex3f(x + nx, y, z + nz);

	glVertex3f(x + nx, y + ny, z);
	glVertex3f(x + nx, y + ny, z + nz);

	glVertex3f(x, y + ny, z);
	glVertex3f(x, y + ny, z + nz);

	//////
	glVertex3f(x, y, z + nz);
	glVertex3f(x + nx, y, z + nz);

	glVertex3f(x + nx, y, z + nz);
	glVertex3f(x + nx, y + ny, z + nz);

	glVertex3f(x + nx, y + ny, z + nz);
	glVertex3f(x, y + ny, z + nz);

	glVertex3f(x, y + ny, z + nz);
	glVertex3f(x, y, z + nz);

	glEnd();

	glPopAttrib();
}

// If one of the boxes should not be rendered, set excludeBox to its index.
// If the main box should not be rendered, set excludeBox to -1.
void Scene::render3D(const QMatrix4x4 &view)
{
	QMatrix4x4 m;
	m.rotate(m_trackBalls[0].rotation());
	loadMatrix(view);

	int nx, ny, nz;
	dataManager->GetVolumeSize(nx, ny, nz);
	int dataDim[3] = { nx, ny, nz };
	//copyDataDim(dataDim, sizeof(int3));

	int qx, qy, qz;
	int qnx, qny, qnz;
	dataManager->GetQCube(qx, qy, qz, qnx, qny, qnz);

	float mindim = std::min(nx, std::min(ny, nz));
	float maxdim = std::max(nx, std::max(ny, nz));

	float minqsize = std::min(qnx, std::min(qny, qnz));
	float minbymax = mindim / maxdim;

	/****** transform********/
	glMultMatrixf(m.constData());

	GLfloat mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	QMatrix4x4 qModelview(
		mv[0], mv[1], mv[2], mv[3],
		mv[4], mv[5], mv[6], mv[7],
		mv[8], mv[9], mv[10], mv[11],
		mv[12], mv[13], mv[14], mv[15]);
	qModelview = qModelview.transposed();

	//glPushMatrix();

	float s = 1.0f / maxdim;
	glScalef(s, s, s);
	//glTranslatef(m_translate[0], m_translate[1], m_translate[2]);
	glTranslatef(-(nx - 1) * 0.5, -(ny - 1) * 0.5, -(nz - 1)* 0.5);

	/****** Volume rendering ******/
	//displayVolume();

	///***** Draw sphere ****/
	//if (glActiveTexture) {
	//	glActiveTexture(GL_TEXTURE0);
	//	m_environment->bind();
	//}


	m_programs["distribution"]->bind();
//	m_programs["distribution"]->setUniformValue("tex", GLint(0));
	m_programs["distribution"]->setUniformValue("env", GLint(0));
	m_programs["distribution"]->setUniformValue("view", qModelview);

#if 0
	//draw sphere by the side
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPushMatrix();
	loadMatrix(view);
	//glLoadIdentity();
	glTranslatef(0.3, -0.25, 0.2);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(0.2, 0.2, 0.2);
	glMultMatrixf(m.constData());
	m_vecWidget->draw();
	glPopMatrix();
	glCullFace(GL_BACK);

	glPushMatrix();
	loadMatrix(view);
	//glLoadIdentity();
	glTranslatef(-0.3, -0.25, 0.2);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(0.2, 0.2, 0.2);
	glMultMatrixf(m.constData());
	m_vecWidget->draw();
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	//draw sphere at the queried region
	glPushMatrix();
	glTranslatef(qnx / 2 + qx, qny / 2 + qy, qnz / 2 + qz);
	glScalef(minqsize, minqsize, minqsize);
	m_vecWidget->draw();
	glPopMatrix();
#endif
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);


	//if (glActiveTexture) {
	//	m_environment->unbind();
	//}

	int dim[3];
	int start[3];
	AbstractNode* selectedNode = nullptr;
	for (int i = 0; i < leafNodes.size(); i++)	{
		GLTextureCube* tex = blockTex[i];
		auto nd = leafNodes[i];
		nd->GetDim(dim);
		nd->GetStart(start);
		glPushMatrix();
		glTranslatef(
			dim[0] / 2 + start[0], 
			dim[1] / 2 + start[1], 
			dim[2] / 2 + start[2]);
		float min_dim = min(min(dim[0], dim[1]), dim[2]);
		glScalef(min_dim, min_dim, min_dim);
		//Scale the size of glyphs
		//glScalef(0.5, 0.5, 0.5);
		tex->bind();
		//m_vecWidget->draw();
		//m_superWidget->draw();
		if (nd->GetVisible()) {
			nd->GetGlyph()->draw();
		}
		//nd->
		
		tex->unbind();

		if (nd->GetSelected()) {
			selectedNode = (AbstractNode*)nd;
			
		}

		glPopMatrix();

		//draw the links
		//for (int j = 0; j < 6; j++)	{
		//	if (nd->flux[j] > 0)	{
		//		float3 c1 = make_float3(
		//			nd->start[0] + nd->dim[0] * 0.5, 
		//			nd->start[1] + nd->dim[1] * 0.5, 
		//			nd->start[2] + nd->dim[2] * 0.5);
		//		float3 c2 = make_float3(
		//			nd->neighbor[j]->start[0] + nd->neighbor[j]->dim[0] * 0.5,
		//			nd->neighbor[j]->start[1] + nd->neighbor[j]->dim[1] * 0.5,
		//			nd->neighbor[j]->start[2] + nd->neighbor[j]->dim[2] * 0.5);
		//		//renderCylinder_convenient(c1.x, c1.y, c1.z, c2.x, c2.y, c2.z, 0.1 * log2( nd->flux[j]), 16);
		//		renderCone_convenient(c1.x, c1.y, c1.z, c2.x, c2.y, c2.z, 0.2 * log2( nd->flux[j]), 16);
		//	}
		//}
	}
	m_programs["distribution"]->release();

	if (nullptr != selectedNode)	{
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
		glColor3f(1.0f, 1.0f, 0.0f); //Currently has no effect
		glLineWidth(6);
		selectedNode->GetStart(start);
		selectedNode->GetDim(dim);
		//renderSelectionBox(view);
		RenderBox(view, start[0], start[1], start[2], dim[0], dim[1], dim[2]);
		glPopAttrib();
	}

//	m_programs["sphere_brush"]->bind();
//	/********Draw for picking******/
//	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//draw sphere at the queried region
//	glPushMatrix();
//	glTranslatef(qnx / 2 + qx, qny / 2 + qy, qnz / 2 + qz);
//	glScalef(minqsize, minqsize, minqsize);
//	//this following line has to be called
//	m_vecWidget->draw();
//	glPopMatrix();
//
//	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	/********end******/
//	m_programs["sphere_brush"]->release();

	/*
	glLineWidth(8.0f);
	float axis_len = 32;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f * axis_len, 0.0f, 0.0f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f * axis_len, 0.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f * axis_len);
	glEnd();
	glLineWidth(1.0f);
	*/

	//draw streamlines
	//glUseProgram(0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	vector<vector<float4>> lines = dataManager->GetStreamlines();
	//TODO: figure out why I have to put the following line
	for (auto line : lines)
	{
		// activate and specify pointer to vertex array
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		//glColorPointer(3, GL_FLOAT, line.size() * sizeof(float3), &colorMap[0]);
		glVertexPointer(4, GL_FLOAT, 0, &(line[0].x));

		// draw a cube
		//TODO: uncomment
		glDrawArrays(GL_LINE_STRIP, 0, line.size());

		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
	}

	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//vector<vector<float4>> linesInCube = dataManager->GetStreamlinesInCube();
	//for (auto line : linesInCube)
	//{
	//	// activate and specify pointer to vertex array
	//	//glEnableClientState(GL_COLOR_ARRAY);
	//	glEnableClientState(GL_VERTEX_ARRAY);

	//	//glColorPointer(3, GL_FLOAT, line.size() * sizeof(float3), &colorMap[0]);
	//	glVertexPointer(4, GL_FLOAT, 0, &line[0]);

	//	// draw a cube
	//	glDrawArrays(GL_LINE_STRIP, 0, line.size());

	//	// deactivate vertex arrays after drawing
	//	glDisableClientState(GL_VERTEX_ARRAY);
	//	//glDisableClientState(GL_COLOR_ARRAY);
	//}

//	/**********draw plane********/
////	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	/***** Draw sphere ****/
//	m_programs["cut_plane"]->bind();
//	if (glActiveTexture) {
//		glActiveTexture(GL_TEXTURE0);
//		m_vec3DTex->bind();
//	}
//	m_programs["cut_plane"]->setUniformValue("tex", GLint(0));
//	m_programs["cut_plane"]->setUniformValue("data_dim", GLfloat(nx), GLfloat(ny), GLfloat(nz));
//
//	for (CutPlane p : cutplanes){
//		//glDrawArrays(GL_TRIANGLE_FAN, 0, );
//		vector<float3> vs = p.vertices;
//		QVector3D plane_normal(p.normal.x, p.normal.y, p.normal.z);
//
//		m_programs["cut_plane"]->setUniformValue("plane_normal",
//			GLfloat(plane_normal.x()), GLfloat(plane_normal.y()), GLfloat(plane_normal.z()));
//
//		glBegin(GL_TRIANGLE_FAN);
//
////	glUniform1fv(glGetUniformLocation(program, "v"), 10, v);
//		for (auto v : vs)
//			glVertex3fv(&v.x);
//		glEnd();
//	}
//	if (glActiveTexture) {
//		m_vec3DTex->unbind();
//	}
//	m_programs["cut_plane"]->release();
//	//	glPolygonMode(GL_FRONT, GL_FILL);


	//renderQCube(view);
	renderBBox(view);
	

	//glPopMatrix();

	//glPushMatrix();
	//glScalef(0.5, 0.5, 0.5);
	//glTranslatef(-1, -1, -1);

	//m_vecWidget->draw();
	//glPopMatrix();



	//glPopAttrib();

}

void Scene::setStates()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);
    //glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    glLoadIdentity();

    //setLights();

    //float materialSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

//void Scene::setLights()
//{
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    //float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
//    float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
//    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
//    //glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
//    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
//    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
//    glEnable(GL_LIGHT0);
//}

void Scene::defaultStates()
{
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDisable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_COLOR_MATERIAL);
    //glDisable(GL_TEXTURE_2D);
   // glDisable(GL_LIGHT0);
    //glDisable(GL_NORMALIZE);

    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();

    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();

    //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
    //float defaultMaterialSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultMaterialSpecular);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

void Scene::drawBackground(QPainter *painter, const QRectF &)
{
	float width = float(painter->device()->width());
	float height = float(painter->device()->height());

	painter->beginNativePainting();
	setStates();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    qgluPerspective(60.0, width / height, 0.01, 15.0);

    glMatrixMode(GL_MODELVIEW);

    QMatrix4x4 view;
//    view.rotate(m_trackBalls[2].rotation());

	//zoom in/out
    view(2, 3) -= 0.5f * exp(m_distExp / 1200.0f);

	render3D(view);


	QString framesPerSecond;
	framesPerSecond.setNum(m_frame / (m_time.elapsed() / 1000.0), 'f', 2);
	painter->setPen(Qt::white);
	//this following line cannot be called when the fix in "glbuffer.h" is made
	//painter->drawText(20, 40, " fps");
	////cout << "FPS: " << framesPerSecond.toStdString() << endl;
	//painter->end();

	if (!(m_frame % 100)) {
		m_time.start();
		m_frame = 0;
	}
	m_frame++;
	defaultStates();
	painter->endNativePainting();
}

QPointF Scene::pixelPosToViewPos(const QPointF& p)
{
    return QPointF(2.0 * float(p.x()) / width() - 1.0,
                   1.0 - 2.0 * float(p.y()) / height());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted())
        return;

    if (event->buttons() & Qt::LeftButton) {
		//we have to use m_trackBalls[2]
        m_trackBalls[0].move(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
        event->accept();
	}
	//else if (event->buttons() & Qt::RightButton) {
	//	m_trackBalls[0].translate(pixelPosToViewPos(event->scenePos()), m_trackBalls[0].rotation().conjugate());
	//	event->accept();
	//}
	else {
        m_trackBalls[0].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[0].rotation().conjugate());
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;

	if (event->button() == Qt::LeftButton) {
        m_trackBalls[0].push(pixelPosToViewPos(event->scenePos()), m_trackBalls[0].rotation().conjugate());
        event->accept();
    }
	else if (event->button() == Qt::RightButton)	{
		//float3 b = ReadPixel(event->scenePos().x(), event->scenePos().y());
		////int3 dim = dataManager->GetVolumeDim();
		////The boundary is from 0 to (dim - 1)
		//int qx, qy, qz;
		//int qnx, qny, qnz;
		//dataManager->GetQCube(qx, qy, qz, qnx, qny, qnz);

		//float3 aabb_min = make_float3(qx, qy, qz);
		//float3 aabb_max = make_float3(qx + qnx - 1, qy + qny - 1, qz + qnz - 1);
		//float3 normal = make_float3(b.x * 2 - 1, b.y * 2 - 1, b.z * 2 - 1);
		//CutPlane cp(normal, dataManager->GetQCubeCenter(), aabb_min, aabb_max);

		//if (cutplanes.size() == 0)
		//	cutplanes.push_back(cp);
		//else
		//	cutplanes.back() = cp;
		//cout << "(" << normal.x << "," << normal.y << "," << normal.z << endl;
	}
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;

    if (event->button() == Qt::LeftButton) {
        m_trackBalls[0].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[0].rotation().conjugate());
        event->accept();
    }
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent * event)
{
    QGraphicsScene::wheelEvent(event);
    if (!event->isAccepted()) {
        m_distExp += event->delta();
        if (m_distExp < -8 * 120)
            m_distExp = -8 * 120;
        if (m_distExp > 10 * 120)
            m_distExp = 10 * 120;
        event->accept();
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
	QGraphicsScene::keyPressEvent(event);
	if (event->key() == Qt::Key_W || event->key() == Qt::Key_S
		|| event->key() == Qt::Key_A || event->key() == Qt::Key_D
		|| event->key() == Qt::Key_Q || event->key() == Qt::Key_E)
	{
		//int change = 50;
		int qx, qy, qz;
		int qnx, qny, qnz;
		dataManager->GetQCube(qx, qy, qz, qnx, qny, qnz);

		int nx, ny, nz;
		dataManager->GetVolumeSize(nx, ny, nz);
		switch (event->key())
		{
		case Qt::Key_W:
			if (qx + qnx * 2 > nx)
				return;
			qx += qnx;
			break;
		case Qt::Key_S:
			if (qx - qnx < 0)
				return;
			qx -= qnx;
			break;
		case Qt::Key_A:
			if (qy + qny * 2 > ny)
				return;
			qy += qny;
			break;
		case Qt::Key_D:
			if (qy - qny < 0)
				return;
			qy -= qny;
			break;
		case Qt::Key_Q:
			if (qz + qnz * 2 > nz)
				return;
			qz += qnz;
			break;
		case Qt::Key_E:
			if (qz - qnz < 0)
				return;
			qz -= qnz;
			break;


		}
		dataManager->SetQCube(qx, qy, qz, qnx, qny, qnz);
		m_renderOptions->setBlock(qx, qy, qz, qnx, qny, qnz);
		UpdateBlock();
		dataManager->GenStreamInCube();
	}
	//switch (event->key())
	//{
	//case Qt::Key_Up:
	//	cutplanes.back().MovePlane(1);
	//	break;
	//case Qt::Key_Down:
	//	cutplanes.back().MovePlane(-1);
	//	break;
	//}
}

void Scene::OnMove(std::vector<float>& motionData)
{
	if (motionData.size() < 6) return;
//	cout << motionData[0] << "," << motionData[1] << "," << motionData[2] << endl;
	dataManager->MoveCube(motionData[0] * 1000, motionData[1] * 1000, motionData[2] * 1000);
//	dataManager->ResizeCube(motionData[3] * 1000, motionData[4] * 1000, motionData[5] * 1000);

	int qx, qy, qz;
	int qnx, qny, qnz;
	dataManager->GetQCube(qx, qy, qz, qnx, qny, qnz);

	m_renderOptions->setBlock(qx, qy, qz, qnx, qny, qnz);
	UpdateBlock();
	//dataManager->GenStreamInCube();
}

//void Scene::setShader(int index)
//{
//    //if (index >= 0 && index < m_fragmentShaders.size())
//    //    m_currentShader = index;
//}

void Scene::setColorParameter(const QString &name, QRgb color)
{
    //// set the color in all programs
    //foreach (QGLShaderProgram *program, m_programs) {
    //    program->bind();
    //    program->setUniformValue(program->uniformLocation(name), QColor(color));
    //    program->release();
    //}
}

void Scene::setFloatParameter(const QString &name, float value)
{
    //// set the color in all programs
    //foreach (QGLShaderProgram *program, m_programs) {
    //    program->bind();
    //    program->setUniformValue(program->uniformLocation(name), value);
    //    program->release();
    //}
}

void Scene::UpdateBlock(int x, int y, int z, int nx, int ny, int nz)
{
	//int size = 32;
	int size = dataManager->GetCubemapSize();
	//	std::unique_ptr<float[]> cubemap(new float[size * size * 6]);
	float* cubemap = nullptr;
	dataManager->GenCubeMap(x, y, z, nx, ny, nz, cubemap);
	m_environment->load(cubemap, size);
}

void Scene::UpdateQuery(int f, int x, int y)
{
////	std::unique_ptr<bool[]> result(new bool[size * size * 6]);
//	int size = dataManager->GetNumOfCells();
//	unsigned char* result = new unsigned char[size];
////	int size = 0;
//	dataManager->QueryByBin(f, x, y, result);
//	int nx, ny, nz;
//	dataManager->GetVolumeSize(nx, ny, nz);
//	cudaExtent volumeSize = make_cudaExtent(nx, ny, nz);
//	inputMask(result, volumeSize);
}

void Scene::UpdateBlock()
{
	//int size = 32;
	int size = dataManager->GetCubemapSize();
	std::unique_ptr<float[]> cubemap(new float[size * size * 6]);
	dataManager->UpdateCubeMap(cubemap.get());
	m_environment->load(cubemap.get(), size);
}

void RenderOptionsDialog::setBlock(int x, int y, int z, int nx, int ny, int nz)
{
	m_blockLoc[0]->setText(QString::number(x));
	m_blockLoc[1]->setText(QString::number(y));
	m_blockLoc[2]->setText(QString::number(z));

	m_blockSize[0]->setText(QString::number(nx));
	m_blockSize[1]->setText(QString::number(ny));
	m_blockSize[2]->setText(QString::number(nz));

	m_blockLoc[0]->setValidator(new QIntValidator(0, nx - 1));
	m_blockLoc[1]->setValidator(new QIntValidator(0, ny - 1));
	m_blockLoc[2]->setValidator(new QIntValidator(0, nz - 1));
}

void RenderOptionsDialog::changeBlockLoc(const int idx, const int val)
{
	m_blockLoc[idx]->setText(QString::number(m_blockLoc[idx]->text().toInt() + val));
}

//void Scene::initPixelBuffer()
//{
//	if (pbo)
//	{
//		// unregister this buffer object from CUDA C
//		checkCudaErrors(cudaGraphicsUnregisterResource(cuda_pbo_resource));
//
//		// delete old buffer
//		glDeleteBuffers(1, &pbo);
//		glDeleteTextures(1, &tex);
//	}
//	// create pixel buffer object for display
//	glGenBuffers(1, &pbo);
//	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
//	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, m_width*m_height*sizeof(GLubyte)* 4, 0, GL_STREAM_DRAW_ARB);
//	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
//
//	// register this buffer object with CUDA
//	checkCudaErrors(cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, cudaGraphicsMapFlagsWriteDiscard));
//
//	// create texture for display
//	glGenTextures(1, &tex);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

void Scene::cleanup()
{
//	//sdkDeleteTimer(&timer);

//	//freeCudaBuffers();

//	if (pbo)
//	{
//		cudaGraphicsUnregisterResource(cuda_pbo_resource);
//		glDeleteBuffers(1, &pbo);
//		glDeleteTextures(1, &tex);
//	}
}

// render image using CUDA
void Scene::renderVolume()
{
//	//copyInvViewMatrix(invViewMatrix, sizeof(float4)* 4);
//	//copyInvProjMatrix(invProjMatrix, sizeof(float4)* 3);
//	copyInvProjMulViewMatrix(invProjMulView, sizeof(float4)* 4);



//	// map PBO to get CUDA device pointer
//	uint *d_output;
//	// map PBO to get CUDA device pointer
//	checkCudaErrors(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
//	size_t num_bytes;
//	checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void **)&d_output, &num_bytes,
//		cuda_pbo_resource));
//	//printf("CUDA mapped PBO: May access %ld bytes\n", num_bytes);

//	// clear image
//	checkCudaErrors(cudaMemset(d_output, 0, m_width*m_height * 4));

//	// call CUDA kernel, writing results to PBO
//	render_kernel(gridSize, blockSize, d_output, m_width, m_height);// , density, brightness, transferOffset, transferScale);

//	getLastCudaError("kernel failed");

//	checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));
}


//// display results using OpenGL (called by GLUT)
//void Scene::displayVolume()
//{
//	//sdkStartTimer(&timer);
//
//	// use OpenGL to build view matrix
//	GLfloat modelView[16];
//	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
//
//	GLfloat projection[16];
//	glGetFloatv(GL_PROJECTION_MATRIX, projection);
//
//	QMatrix4x4 q_modelview(
//		modelView[0], modelView[4], modelView[8], modelView[12], 
//		modelView[1], modelView[5], modelView[9], modelView[13],
//		modelView[2], modelView[6], modelView[10], modelView[14],
//		modelView[3], modelView[7], modelView[11], modelView[15]
//		);
//
//	QMatrix4x4 q_projection(
//		projection[0], projection[4], projection[8], projection[12],
//		projection[1], projection[5], projection[9], projection[13],
//		projection[2], projection[6], projection[10], projection[14],
//		projection[3], projection[7], projection[11], projection[15]
//		);
//
//	QMatrix4x4 q_invProjMulView = (q_projection * q_modelview).inverted();
//	//GLfloat projMulView[16];
//	//MultiplyMatrices4by4OpenGL_FLOAT(projMulView, projection, modelView);
//
//	//glhInvertMatrixf2(projMulView, invProjMulView);
//	q_invProjMulView.copyDataTo(invProjMulView);
//	//invProjMulView 
//
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	//glRotatef(-viewRotation.x, 1.0, 0.0, 0.0);
//	//glRotatef(-viewRotation.y, 0.0, 1.0, 0.0);
//	//glTranslatef(-viewTranslation.x, -viewTranslation.y, -viewTranslation.z);
//
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
//
//
//	renderVolume();
//
//	// display results
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glPushAttrib(GL_DEPTH_TEST);
//	// draw image from PBO
//	glDisable(GL_DEPTH_TEST);
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//#if 0
//	// draw using glDrawPixels (slower)
//	glRasterPos2i(0, 0);
//	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
//	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
//#else
//	// draw using texture
//
//	// copy from pbo to texture
//	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
//
//	// draw textured quad
//	glEnable(GL_TEXTURE_2D);
//	glBegin(GL_QUADS);
//	glTexCoord2f(0, 0);
//	glVertex2f(0, 0);
//	glTexCoord2f(1, 0);
//	glVertex2f(1, 0);
//	glTexCoord2f(1, 1);
//	glVertex2f(1, 1);
//	glTexCoord2f(0, 1);
//	glVertex2f(0, 1);
//	glEnd();
//
//	glDisable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	glPopAttrib();
//#endif
//
//	//glutSwapBuffers();
//	//glutReportErrors();
//
//	//sdkStopTimer(&timer);
//	//glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//
//	//computeFPS();
//}
//
//void Scene::DrawPicking()
//{
//
//}


////color picking: http://ogldev.atspace.co.uk/www/tutorial29/tutorial29.html
//bool Scene::InitPicking(unsigned int WindowWidth, unsigned int WindowHeight)
//{
//	// Create the FBO
//	glGenFramebuffersEXT(1, &m_fbo);
//	glBindFramebufferEXT(GL_FRAMEBUFFER, m_fbo);
//
//	// Create the texture object for the primitive information buffer
//	glGenTextures(1, &m_pickingTexture);
//	glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight,
//		0, GL_RGB, GL_FLOAT, NULL);
//	glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
//		m_pickingTexture, 0);
//
//	// Create the texture object for the depth buffer
//	glGenTextures(1, &m_depthTexture);
//	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
//		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
//		m_depthTexture, 0);
//
//	// Disable reading to avoid problems with older GPUs
//	glReadBuffer(GL_NONE);
//
//	glDrawBuffer(GL_COLOR_ATTACHMENT0);
//
//	// Verify that the FBO is correct
//	GLenum Status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
//
//	if (Status != GL_FRAMEBUFFER_COMPLETE) {
//		printf("FB error, status: 0x%x\n", Status);
//		return false;
//	}
//
//	// Restore the default framebuffer
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
//
//	return 0;// GLCheckError();
//}
//
//float3 Scene::ReadPixel(unsigned int x, unsigned int y)
//{
//	glBindFramebufferEXT(GL_READ_FRAMEBUFFER, m_fbo);
//	glReadBuffer(GL_COLOR_ATTACHMENT0);
//	cout << "x,y: " << x << "," << y << endl;
//
//	float3 Pixel;
//	glReadPixels(x, m_height - y - 1, 1, 1, GL_RGB, GL_FLOAT, &Pixel);
//
//	glReadBuffer(GL_NONE);
//	glBindFramebufferEXT(GL_READ_FRAMEBUFFER, 0);
//
//	return Pixel;
//}

void Scene::Segmentation()
{
	//dataManager->Segmentation();
//	dataManager->LoadSegmentation();
}

//CutPlane::CutPlane(float3 _normal, float3 _vertex, float3 _aabb_min, float3 _aabb_max)
//{
//	this->normal = _normal;
//	this->aabb_min = _aabb_min;
//	this->aabb_max = _aabb_max;
//	plane_coef = make_float4(normal.x, normal.y, normal.z, 
//		- normal.x * _vertex.x - normal.y * _vertex.y - normal.z * _vertex.z);
//	float3 out_points[6];
//	unsigned int out_point_count = 0;
//	calc_plane_aabb_intersection_points(plane_coef, aabb_min, aabb_max, out_points, out_point_count);
//	sort_points(out_points, out_point_count, plane_coef);
//	for (int i = 0; i < out_point_count; i++)	{
//		vertices.push_back(out_points[i]);
//	}
//}
//
//void CutPlane::MovePlane(float v)
//{
//	float3 out_points[6];
//	unsigned int out_point_count = 0;
//	plane_coef.w += v;
//	calc_plane_aabb_intersection_points(plane_coef, aabb_min, aabb_max, out_points, out_point_count);
//	sort_points(out_points, out_point_count, plane_coef);
//	vertices.clear();
//	for (int i = 0; i < out_point_count; i++)	{
//		vertices.push_back(out_points[i]);
//	}
//
//}
//

void Scene::ShowGpuMemInfo()
{



//	// show memory usage of GPU

//	size_t free_byte;

//	size_t total_byte;


//	cudaError_t cuda_status = cudaMemGetInfo(&free_byte, &total_byte);

//	if (cudaSuccess != cuda_status){

//		printf("Error: cudaMemGetInfo fails, %s \n", cudaGetErrorString(cuda_status));

//		exit(1);

//	}



//	double free_db = (double)free_byte;

//	double total_db = (double)total_byte;

//	double used_db = total_db - free_db;

//	printf("GPU memory usage: used = %f, free = %f MB, total = %f MB\n",

//		used_db / 1024.0 / 1024.0, free_db / 1024.0 / 1024.0, total_db / 1024.0 / 1024.0);
}
