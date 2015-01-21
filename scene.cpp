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

//#include "3rdparty/fbm.h"
#include <memory>
#include <driver_types.h>
#include <driver_functions.h>
//#include <qopenglext.h>

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

#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a) = (b); (b) = _tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

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

//============================================================================//
//                           TwoSidedGraphicsWidget                           //
//============================================================================//

TwoSidedGraphicsWidget::TwoSidedGraphicsWidget(QGraphicsScene *scene)
    : QObject(scene)
    , m_current(0)
    , m_angle(0)
    , m_delta(0)
{
    for (int i = 0; i < 2; ++i)
        m_proxyWidgets[i] = 0;
}

void TwoSidedGraphicsWidget::setWidget(int index, QWidget *widget)
{
    if (index < 0 || index >= 2)
    {
        qWarning("TwoSidedGraphicsWidget::setWidget: Index out of bounds, index == %d", index);
        return;
    }

    GraphicsWidget *proxy = new GraphicsWidget;
    proxy->setWidget(widget);

    if (m_proxyWidgets[index])
        delete m_proxyWidgets[index];
    m_proxyWidgets[index] = proxy;

    proxy->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    proxy->setZValue(1e30); // Make sure the dialog is drawn on top of all other (OpenGL) items

    if (index != m_current)
        proxy->setVisible(false);

    qobject_cast<QGraphicsScene *>(parent())->addItem(proxy);
}

QWidget *TwoSidedGraphicsWidget::widget(int index)
{
    if (index < 0 || index >= 2)
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

Scene::Scene(int width, int height, int maxTextureSize)
    : m_distExp(600)
    , m_frame(0)
    , m_maxTextureSize(maxTextureSize)
    , m_currentShader(0)
    //, m_currentTexture(0)
    , m_dynamicCubemap(false)
    //, m_updateAllCubemaps(true)
    , m_vecWidget(0)
    , m_vertexShader(0)
    //, m_environmentShader(0)
    //, m_environmentProgram(0)
{
	dataManager = new DataManager();
	//$$$
	
	//dataManager->LoadVec("D:/data/sample/test2.vec");
		//dataManager->LoadVec("D:/data/sample/test1.vec");
	dataManager->LoadVec("C:/Users/tong.tong-idea/SkyDrive/share/15plume3d430.vec");
	//dataManager->LoadVec("D:/data/plume/15plume3d421.vec");
	//dataManager->LoadVec("D:/data/isabel/UVWf01.vec");
	m_width = width;
	m_height = height;
	blockSize.x = 16;
	blockSize.y = 16;
	gridSize = dim3(iDivUp(width, blockSize.x), iDivUp(height, blockSize.y));


    setSceneRect(0, 0, width, height);

    m_trackBalls[0] = TrackBall(0.05f, QVector3D(0, 1, 0), TrackBall::Sphere);
    //m_trackBalls[1] = TrackBall(0.005f, QVector3D(0, 0, 1), TrackBall::Sphere);
    m_trackBalls[2] = TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::Plane);

    m_renderOptions = new RenderOptionsDialog;
    m_renderOptions->move(20, 120);
    m_renderOptions->resize(m_renderOptions->sizeHint());

	int nx, ny, nz;
	dataManager->GetVolumeSize(nx, ny, nz);
	m_renderOptions->setBlock(0, 0, 0, nx, ny, nz);

	initPixelBuffer();
	
    connect(m_renderOptions, SIGNAL(colorParameterChanged(QString,QRgb)), this, SLOT(setColorParameter(QString,QRgb)));
    connect(m_renderOptions, SIGNAL(floatParameterChanged(QString,float)), this, SLOT(setFloatParameter(QString,float)));
    connect(m_renderOptions, SIGNAL(shaderChanged(int)), this, SLOT(setShader(int)));
	connect(m_renderOptions, SIGNAL(blockChanged(int, int, int, int, int, int)), 
		this, SLOT(UpdateBlock(int, int, int, int, int, int)));
	connect(m_renderOptions, SIGNAL(queryChanged(int, int, int)), this, SLOT(UpdateQuery(int, int, int)));
	connect(m_renderOptions, SIGNAL(segmentationRequested()), this, SLOT(Segmentation()));

    TwoSidedGraphicsWidget *twoSided = new TwoSidedGraphicsWidget(this);
    twoSided->setWidget(0, m_renderOptions);

    initGL();

    m_timer = new QTimer(this);
    m_timer->setInterval(20);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start();

    m_time.start();
}

Scene::~Scene()
{
	if (m_vecWidget)
        delete m_vecWidget;
    foreach (GLTexture *texture, m_textures)
        if (texture) delete texture;
    foreach (QGLShaderProgram *program, m_programs)
        if (program) delete program;
    if (m_vertexShader)
        delete m_vertexShader;
    foreach (QGLShader *shader, m_fragmentShaders)
        if (shader) delete shader;
    foreach (GLRenderTargetCube *rt, m_cubemaps)
        if (rt) delete rt;

	cleanup();
	cudaDeviceReset();
}

void Scene::initGL()
{
	m_vecWidget = new GLSphere(1.0f, 0.5f, 10);

    m_vertexShader = new QGLShader(QGLShader::Vertex);
    m_vertexShader->compileSourceFile(QLatin1String(":/res/boxes/basic.vsh"));

	m_environment = new GLTextureCube(qMin(1024, m_maxTextureSize), 1);

    QStringList filter;
    QList<QFileInfo> files;

    // Load all .fsh files as fragment shaders
    m_currentShader = 0;
    filter = QStringList("*.fsh");
    files = QDir(":/res/boxes/").entryInfoList(filter, QDir::Files | QDir::Readable);
    foreach (QFileInfo file, files) {
        QGLShaderProgram *program = new QGLShaderProgram;
        QGLShader* shader = new QGLShader(QGLShader::Fragment);
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
        m_programs << program;
        m_renderOptions->addShader(file.baseName());

        program->bind();
        m_cubemaps << ((program->uniformLocation("env") != -1) ? new GLRenderTargetCube(qMin(256, m_maxTextureSize)) : 0);
        program->release();
    }

    if (m_programs.size() == 0)
        m_programs << new QGLShaderProgram;

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
	
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(nx, 0, 0);

	glVertex3f(nx, 0, 0);
	glVertex3f(nx, ny, 0);

	glVertex3f(nx, ny, 0);
	glVertex3f(0, ny, 0);

	glVertex3f(0, ny, 0);
	glVertex3f(0, 0, 0);

	//////
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, nz);

	glVertex3f(nx, 0, 0);
	glVertex3f(nx, 0, nz);

	glVertex3f(nx, ny, 0);
	glVertex3f(nx, ny, nz);

	glVertex3f(0, ny, 0);
	glVertex3f(0, ny, nz);

	//////
	glVertex3f(0, 0, nz);
	glVertex3f(nx, 0, nz);

	glVertex3f(nx, 0, nz);
	glVertex3f(nx, ny, nz);

	glVertex3f(nx, ny, nz);
	glVertex3f(0, ny, nz);

	glVertex3f(0, ny, nz);
	glVertex3f(0, 0, nz);

	glEnd();
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
void Scene::render3D(const QMatrix4x4 &view, int excludeBox)
{
	QMatrix4x4 m;
	m.rotate(m_trackBalls[0].rotation());
	loadMatrix(view);



	int nx, ny, nz;
	dataManager->GetVolumeSize(nx, ny, nz);
	int dataDim[3] = { nx, ny, nz };
	copyDataDim(dataDim, sizeof(int3));

	int qx, qy, qz;
	int qnx, qny, qnz;
	dataManager->GetQCube(qx, qy, qz, qnx, qny, qnz);

	float mindim = std::min(nx, std::min(ny, nz));
	float maxdim = std::max(nx, std::max(ny, nz));

	float minqsize = std::min(qnx, std::min(qny, qnz));
	float minbymax = mindim / maxdim;

	/****** transform********/
	glMultMatrixf(m.constData());


	//glPushMatrix();

	float s = 1.0f / maxdim;
	glScalef(s, s, s);
	//glTranslatef(m_translate[0], m_translate[1], m_translate[2]);
	glTranslatef(-(0 + nx / 2), -(0 + ny / 2), -(0 + nz / 2));

	/****** Volume rendering ******/
	displayVolume();

	/***** Draw sphere ****/
	if (glActiveTexture) {
		glActiveTexture(GL_TEXTURE0);
		m_environment->bind();
	}

	m_programs[m_currentShader]->bind();
//	m_programs[m_currentShader]->setUniformValue("tex", GLint(0));
	m_programs[m_currentShader]->setUniformValue("env", GLint(0));


	glPushMatrix();
	loadMatrix(view);
	//glLoadIdentity();
	glTranslatef(0.3, -0.25, 0.2);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(0.2, 0.2, 0.2);
	glMultMatrixf(m.constData());
	m_vecWidget->draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(qnx / 2 + qx, qny / 2 + qy, qnz / 2 + qz);
	glScalef(minqsize, minqsize, minqsize);
	//m_vecWidget->draw();
	glPopMatrix();

	vector<Node*> nodes = dataManager->GetAllNode();
	for (auto nd : nodes)	{
		glPushMatrix();
		glTranslatef(
			nd->dim[0] / 2 + nd->start[0], 
			nd->dim[1] / 2 + nd->start[1], 
			nd->dim[2] / 2 + nd->start[2]);
		glScalef(nd->dim[0], nd->dim[0], nd->dim[0]);
		glScalef(0.5, 0.5, 0.5);
		m_vecWidget->draw();
		glPopMatrix();
	}

	m_programs[m_currentShader]->release();

	if (glActiveTexture) {
		m_environment->unbind();
	}


	renderQCube(view);
	renderBBox(view);

	//glPopMatrix();

	//glPushMatrix();
	//glScalef(0.5, 0.5, 0.5);
	//glTranslatef(-1, -1, -1);

	//m_vecWidget->draw();
	//glPopMatrix();



	glPopAttrib();

}

void Scene::setStates()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    setLights();

    float materialSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

void Scene::setLights()
{
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
    glEnable(GL_LIGHT0);
}

void Scene::defaultStates()
{
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
    float defaultMaterialSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultMaterialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
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

    defaultStates();
    ++m_frame;

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
        m_trackBalls[0].move(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
        event->accept();
	}
	else if (event->buttons() & Qt::LeftButton) {
		m_trackBalls[0].translate(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}
	else {
        m_trackBalls[0].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;

	if (event->buttons() & (Qt::LeftButton | Qt::RightButton)) {
        m_trackBalls[0].push(pixelPosToViewPos(event->scenePos()), m_trackBalls[0].rotation().conjugate());
        event->accept();
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

	}

}

void Scene::setShader(int index)
{
    if (index >= 0 && index < m_fragmentShaders.size())
        m_currentShader = index;
}

void Scene::setColorParameter(const QString &name, QRgb color)
{
    // set the color in all programs
    foreach (QGLShaderProgram *program, m_programs) {
        program->bind();
        program->setUniformValue(program->uniformLocation(name), QColor(color));
        program->release();
    }
}

void Scene::setFloatParameter(const QString &name, float value)
{
    // set the color in all programs
    foreach (QGLShaderProgram *program, m_programs) {
        program->bind();
        program->setUniformValue(program->uniformLocation(name), value);
        program->release();
    }
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
//	std::unique_ptr<bool[]> result(new bool[size * size * 6]);
	int size = dataManager->GetNumOfCells();
	unsigned char* result = new unsigned char[size];
//	int size = 0;
	dataManager->QueryByBin(f, x, y, result);
	int nx, ny, nz;
	dataManager->GetVolumeSize(nx, ny, nz);
	cudaExtent volumeSize = make_cudaExtent(nx, ny, nz);
	inputMask(result, volumeSize);
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

void Scene::initPixelBuffer()
{
	if (pbo)
	{
		// unregister this buffer object from CUDA C
		checkCudaErrors(cudaGraphicsUnregisterResource(cuda_pbo_resource));

		// delete old buffer
		glDeleteBuffers(1, &pbo);
		glDeleteTextures(1, &tex);
	}
	// create pixel buffer object for display
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, m_width*m_height*sizeof(GLubyte)* 4, 0, GL_STREAM_DRAW_ARB);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

	// register this buffer object with CUDA
	checkCudaErrors(cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, cudaGraphicsMapFlagsWriteDiscard));

	// create texture for display
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Scene::cleanup()
{
	//sdkDeleteTimer(&timer);

	freeCudaBuffers();

	if (pbo)
	{
		cudaGraphicsUnregisterResource(cuda_pbo_resource);
		glDeleteBuffers(1, &pbo);
		glDeleteTextures(1, &tex);
	}
}

// render image using CUDA
void Scene::renderVolume()
{
	//copyInvViewMatrix(invViewMatrix, sizeof(float4)* 4);
	//copyInvProjMatrix(invProjMatrix, sizeof(float4)* 3);
	copyInvProjMulViewMatrix(invProjMulView, sizeof(float4)* 4);



	// map PBO to get CUDA device pointer
	uint *d_output;
	// map PBO to get CUDA device pointer
	checkCudaErrors(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
	size_t num_bytes;
	checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void **)&d_output, &num_bytes,
		cuda_pbo_resource));
	//printf("CUDA mapped PBO: May access %ld bytes\n", num_bytes);

	// clear image
	checkCudaErrors(cudaMemset(d_output, 0, m_width*m_height * 4));

	// call CUDA kernel, writing results to PBO
	render_kernel(gridSize, blockSize, d_output, m_width, m_height);// , density, brightness, transferOffset, transferScale);

	getLastCudaError("kernel failed");

	checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));
}


// display results using OpenGL (called by GLUT)
void Scene::displayVolume()
{
	//sdkStartTimer(&timer);

	// use OpenGL to build view matrix
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

	GLfloat projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection);

	QMatrix4x4 q_modelview(
		modelView[0], modelView[4], modelView[8], modelView[12], 
		modelView[1], modelView[5], modelView[9], modelView[13],
		modelView[2], modelView[6], modelView[10], modelView[14],
		modelView[3], modelView[7], modelView[11], modelView[15]
		);

	QMatrix4x4 q_projection(
		projection[0], projection[4], projection[8], projection[12],
		projection[1], projection[5], projection[9], projection[13],
		projection[2], projection[6], projection[10], projection[14],
		projection[3], projection[7], projection[11], projection[15]
		);

	QMatrix4x4 q_invProjMulView = (q_projection * q_modelview).inverted();
	//GLfloat projMulView[16];
	//MultiplyMatrices4by4OpenGL_FLOAT(projMulView, projection, modelView);

	//glhInvertMatrixf2(projMulView, invProjMulView);
	q_invProjMulView.copyDataTo(invProjMulView);
	//invProjMulView 

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//glRotatef(-viewRotation.x, 1.0, 0.0, 0.0);
	//glRotatef(-viewRotation.y, 0.0, 1.0, 0.0);
	//glTranslatef(-viewTranslation.x, -viewTranslation.y, -viewTranslation.z);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);


	renderVolume();

	// display results
	glClear(GL_COLOR_BUFFER_BIT);

	glPushAttrib(GL_DEPTH_TEST);
	// draw image from PBO
	glDisable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#if 0
	// draw using glDrawPixels (slower)
	glRasterPos2i(0, 0);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
#else
	// draw using texture

	// copy from pbo to texture
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

	// draw textured quad
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);
	glVertex2f(1, 0);
	glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(0, 1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopAttrib();
#endif

	//glutSwapBuffers();
	//glutReportErrors();

	//sdkStopTimer(&timer);
	//glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//computeFPS();
}

//color picking: http://ogldev.atspace.co.uk/www/tutorial29/tutorial29.html
bool Scene::InitPicking(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the FBO
	glGenFramebuffersEXT(1, &m_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER, m_fbo);

	// Create the texture object for the primitive information buffer
	glGenTextures(1, &m_pickingTexture);
	glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight,
		0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		m_pickingTexture, 0);

	// Create the texture object for the depth buffer
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		m_depthTexture, 0);

	// Disable reading to avoid problems with older GPUs
	glReadBuffer(GL_NONE);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Verify that the FBO is correct
	GLenum Status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	// Restore the default framebuffer
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	return 0;// GLCheckError();
}

Bin Scene::ReadPixel(unsigned int x, unsigned int y)
{
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	Bin Pixel;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &Pixel);

	glReadBuffer(GL_NONE);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER, 0);

	return Pixel;
}

void Scene::Segmentation()
{
	dataManager->Segmentation();
}
