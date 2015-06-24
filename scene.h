/*****************************************************************************
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

#ifndef SCENE_H
#define SCENE_H

//#include <GL/glew.h>
#include "glextensions.h"
#include "graphWidget.h"
#include "qjsonview.h"
#include "TreeMapWindow.h"
//#include "GraphVizWidget.h"

#include <QtWidgets>
#include <QtOpenGL>
#include <QtGui>

#include "glsphere.h"
#include "glSuperquadric.h"
//#include "box.h"
#include "gltrianglemesh.h"
#include "trackball.h"
#include "glbuffers.h"
//#include "DataMgrVect.h"
#include "DataMgrCosm.h"
#include "TextureCubeManager.h"
#include "ArrowButton.h"
#include "ArrowWidget.h"
#include "AppSettings.h"
#include "EntropySlider.h"
#include "EntropyQueryContainer.h"
//#include "qtbox.h"
//#include <cuda_runtime.h>
//#include <cuda_gl_interop.h>
// CUDA utilities
//#include <helper_cuda.h>
//#include <helper_cuda_gl.h>

// Helper functions
//#include <helper_cuda.h>
//#include <helper_functions.h>
//#include <helper_timer.h>
#include <QLinearGradient>
#include <QTimer>
#include <qelapsedtimer.h>

#define PI 3.14159265358979

QT_BEGIN_NAMESPACE
class QMatrix4x4;
QT_END_NAMESPACE



struct Bin {
	unsigned char f;
	unsigned char x;
	unsigned char y;

	Bin()     {
		f = 0;
		x = 0;
		y = 0;
	}
};

//struct CutPlane{
//	//float3 center;
//	float dis2orig;
//	float3 normal;
//	float3 aabb_min, aabb_max;
//	vector < float3 > vertices;
//	float4 plane_coef;
//	CutPlane(float3 _normal, float3 _vertex, float3 _aabb_min, float3 _aabb_max);
//	void MovePlane(float v);
//};
//

class ParameterEdit : public QWidget
{
public:
	virtual void emitChange() = 0;
};

//class FloatEdit : public ParameterEdit
//{
//    Q_OBJECT
//public:
//    FloatEdit(float initialValue, int id);
//    float value() const {return m_value;}
//    virtual void emitChange() {emit valueChanged(m_value, m_id);}
//public slots:
//    void editDone();
//signals:
//    void valueChanged(float value, int id);
//private:
//    QGraphicsScene *m_dialogParentScene;
//    QLineEdit *m_lineEdit;
//    float m_value;
//    int m_id;
//};

class GraphicsWidget : public QGraphicsProxyWidget
{
public:
    GraphicsWidget() : QGraphicsProxyWidget(0, Qt::Window) {}
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class TwoSidedGraphicsWidget : public QObject
{
    Q_OBJECT
public:
    TwoSidedGraphicsWidget(QGraphicsScene *scene);
    void setWidget(int index, QWidget *widget);
    QWidget *widget(int index);
public slots:
    void flip();
protected slots:
    void animateFlip();
private:
    GraphicsWidget *m_proxyWidgets[7];
    int m_current;
    int m_angle; // angle in degrees
    int m_delta;
};

class RenderOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    RenderOptionsDialog();
    //int addTexture(const QString &name);
    int addShader(const QString &name);
    void emitParameterChanged();
	void changeBlockLoc(const int idx, const int val);

public slots:
	void setBlock(int x, int y, int z, int nx, int ny, int nz);
	void updateBlock();
	void doQuery();
	void doSegmentation();

protected slots:
    void setColorParameter(QRgb color, int id);
    void setFloatParameter(float value, int id);
signals:
    void dynamicCubemapToggled(int);
    void colorParameterChanged(const QString &, QRgb);
    void floatParameterChanged(const QString &, float);
    void textureChanged(int);
    void shaderChanged(int);
    void doubleClicked();
	void blockChanged(int, int, int, int, int, int);
	void queryChanged(int, int, int);
	void segmentationRequested();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    QVector<QByteArray> m_parameterNames;
    QComboBox *m_textureCombo;
    QComboBox *m_shaderCombo;
	QVector<ParameterEdit *> m_parameterEdits;
	QVector<QLineEdit *> m_blockLoc;
	QVector<QLineEdit *> m_blockSize;
	QPushButton *m_updateButton;
	QPushButton *m_query;
	QPushButton *m_segmentation;

};
//
//class ItemDialog : public QDialog
//{
//    Q_OBJECT
//public:
//    enum ItemType {
//        QtBoxItem,
//        CircleItem,
//        SquareItem,
//    };
//
//    ItemDialog();
//public slots:
////    void triggerNewQtBox();
////    void triggerNewCircleItem();
////    void triggerNewSquareItem();
//signals:
//    void doubleClicked();
////    void newItemTriggered(ItemDialog::ItemType type);
//protected:
//    virtual void mouseDoubleClickEvent(QMouseEvent *event);
//};

class QJsonView;

class Scene : public QGraphicsScene, protected QGLFunctions
{
    Q_OBJECT
public:
    Scene(int width, int height, int maxTextureSize);
    ~Scene();
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
	//TODO: Remove this after JSON Component is either removed or revised:
	void UpdateTexture() { m_textureCubeManager->UpdateTexture(application); }
	void UpdateBlock();
	bool initiateEntropyQuery(double threshold);
	void adjustPickingTextureForResize(int width, int height);


public slots:
    //void setShader(int index);
    //void setTexture(int index);
    //void toggleDynamicCubemap(int state);
    void setColorParameter(const QString &name, QRgb color);
	void setFloatParameter(const QString &name, float value);
	void UpdateBlock(int x, int y, int z, int nx, int ny, int nz);
	void UpdateQuery(int f, int x, int y);
	void Segmentation();
	void OnMove(std::vector<float>& motionData);
//    void newItem(ItemDialog::ItemType type);
	void dropBoxSelection();
	void sliderSelection(int newValue);
	void RenderBox(const QMatrix4x4 &view, int sx, int sy, int sz, int nx, int ny, int nz);
	//NewColor Newlight
	void setCurrentColormapID(int id);
	//NewColor Newlight over
protected:
	void render3D(const QMatrix4x4 &view);
	void renderBBox(const QMatrix4x4 &view);
	//void renderSelectionBox(const QMatrix4x4 &view);
	void renderQCube(const QMatrix4x4 &view);	//render the queried cube
	void setSelectionBoxPosition(int x, int y, int z);
	void setSelectionBoxWidth(int width);
	void setStates();
    //void setLights();
    void defaultStates();
    //void renderCubemaps();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);
private:
    void initGL();
    QPointF pixelPosToViewPos(const QPointF& p);
	//void initPixelBuffer();
	void cleanup();
	//void displayVolume();
	void renderVolume();
	//bool InitPicking(unsigned int WindowWidth, unsigned int WindowHeight);
	//float3 ReadPixel(unsigned int x, unsigned int y);
	//void DrawPicking();
	void ShowGpuMemInfo();

    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;
    int m_distExp;
    int m_frame;
    int m_maxTextureSize;

	//new translation
	double translateX;
	double translateY;
	int preMouseX;
	int preMouseY;
	int curMouseX;
	int curMouseY;
	//new translation over

    //int m_currentShader;
    //int m_currentTexture;
    bool m_dynamicCubemap;
    //bool m_updateAllCubemaps;

	AppSettings * appSettings = nullptr;

    RenderOptionsDialog *m_renderOptions;
	TreeMapWindow * treeMapWindow;
	GraphWidget * m_graphWidget;
	QJsonView * m_jsonView;
	QScrollArea * scrollArea = nullptr;
	QListWidget * m_listWidget;
	EntropySlider * sliderWidget;
	EntropyQueryContainer * entropyQueryContainer;
	//GraphVizWidget * m_graphVizWidget;

	//ArrowWidget * arrowWidget;
	

    //ItemDialog *m_itemDialog;
    QTimer *m_timer;
	GLSphere *m_vecWidget;
	GLSuperquadric *m_superWidget;
    TrackBall m_trackBalls[3];
    //QVector<GLTexture *> m_textures;
	TextureCubeManager * m_textureCubeManager;
    GLTextureCube *m_environment;	//the used cubemap
	//GLTexture3D *m_vec3DTex;
    //GLTexture3D *m_noise;
    //GLRenderTargetCube *m_mainCubemap;
    //QVector<GLRenderTargetCube *> m_cubemaps;

    map<string, QGLShaderProgram *> m_programs;
    QGLShader *m_vertexShader;
    QVector<QGLShader *> m_fragmentShaders;
    //QGLShader *m_environmentShader;
    //QGLShaderProgram *m_environmentProgram;

	//NewColor Newlight
	QVector4D colmap[33];
	int currentColormapID;
	void readColormap();
	//NewColor Newlight over

	DataManager* dataManager;
	vector<MergeNode *> levelXForest;  //The forest as built from nodes at time step X.  TO DO: Move this into the data manager, and unify it with the representation of the larger forest from the data file

    GLuint pbo;     // OpenGL pixel buffer object
	GLuint tex;     // OpenGL texture object
	struct cudaGraphicsResource *cuda_pbo_resource; // CUDA Graphics Resource (to transfer PBO)
	int m_width;
	int m_height;
	float invViewMatrix[12];
	float invProjMatrix[12];
	dim3 gridSize;
	dim3 blockSize;
	GLfloat invProjMulView[16];

	//TO DO: Remove these unless we end up using them to represent the selection box position
	double selectionX = 0;
	double selectionY = 0;
	double selectionZ = 0;
	double selectionBoxWidth = 0.6;

	////picking
	//GLuint m_fbo;
	//GLuint m_pickingTexture;
	//GLuint m_depthTexture;

	//
	vector<float3> colorMap;
	//vector<CutPlane> cutplanes;

	//Declarations From http://ogldev.atspace.co.uk/www/tutorial29/tutorial29.html
	//GNU License
	////////////////////////////////////////
	GLuint m_fbo;
	GLuint m_pickingTexture;
	GLuint m_depthTexture;
	////////////////////////////////////////

	//FPS Variables
	QElapsedTimer fpsTimer;		//The timer to use for the FPS calculations
	long summedTime = 0;
	//long startTime = 0;			//Start time
	//long lastPrintTime = 0;		//Time at which FPS was last reported
	long frameCount = 0;		//Total accrued frames
	const int FRAME_COUNT_LIMIT = 1000;
};

#endif
