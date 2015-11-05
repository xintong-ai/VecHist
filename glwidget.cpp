//#include "GL/glew.h"
//#include <vector_types.h>
//#include <vector_functions.h>
//#include "helper_math.h"

#include "glwidget.h"

#include "vector_types.h"
#include "vector_functions.h"
#include "helper_math.h"

//#include <vector_functions.h>

#include <iostream>
#include <fstream>
#include <helper_timer.h>
#include <Renderable.h>
#include <LineRenderable.h>

//#include <GL/glu.h>
#include <Trackball.h>
#include <Rotation.h>

//#include <DataMgr.h>
//#include <GLLensRenderable.h>
//#include <GLLineRenderable.h>
//#include <TexPlaneRenderable.h>
//#include <ShaderProgram.h>
//#include <cuda_math.h>
//#include "QtCore/qloggingcategory.h"
//inline float3 operator+(float3 a, float3 b)
//{
//	return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
//}
//
//inline float3 operator*(float3 a, float b)
//{
//	return make_float3(a.x * b, a.y * b, a.z * b);
//}
//
//
//inline float3 operator-(float3 a, float3 b)
//{
//	return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
//}
//
//inline float3 operator*(float b, float3 a)
//{
//	return make_float3(b * a.x, b * a.y, b * a.z);
//}

#if USE_PBO
#include <helper_cuda.h>
#include <cuda_gl_interop.h>
#endif

void GLWidget::printModelView()
{

    glMatrixMode(GL_MODELVIEW);
    GLfloat modelView[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

    std::fstream fs;
    fs.open ("parameters.txt", std::fstream::out);

    fs <<"modelview matrix by glGetFloatv(GL_MODELVIEW_MATRIX, modelView):\n";
    for(int i = 0; i < 16; i++)
        fs << modelView[i]<< " ";
    fs << std::endl;

    fs <<"window size:"<< width<<" "<<height<<"\n";
    fs <<"the smaller z value is, the farther the point is from the camera"<<"\n";
    fs <<"image file format: ABGR ABGR ABGR"<<"\n";

    fs.close();
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_frame(0)
{
    setFocusPolicy(Qt::StrongFocus);
    sdkCreateTimer(&timer);

    trackball = new Trackball();
    rot = new Rotation();

    transRot.setToIdentity();

    //QTimer *aTimer = new QTimer;
    //connect(aTimer,SIGNAL(timeout()),SLOT(animate()));
    //aTimer->start(17);

	//QList<Qt::GestureType> gestures;
	//if (!commandLineParser.isSet(disablePanOption))
		//gestures << Qt::PanGesture;
	//if (!commandLineParser.isSet(disablePinchOption))
		//gestures << Qt::PinchGesture;
	//if (!commandLineParser.isSet(disableSwipeOption))
		//gestures << Qt::SwipeGesture;
		//foreach(Qt::GestureType gesture, gestures)
	grabGesture(Qt::PinchGesture);

	//oneFingerTimer.start();
}

//void GLWidget::animate()
//{
//	//if (!doAnimating)
//	//	return;
////	((GLLineRenderable*)GetRenderable("tract"))->animate();
//    update();
//}

void GLWidget::AddRenderable(const char* name, void* r)
{
	renderers[name] = (Renderable*)r;
	((Renderable*)r)->SetAllRenderable(&renderers);
	((Renderable*)r)->SetActor(this);
	((Renderable*)r)->SetWindowSize(width, height);

}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(256, 256);
}

QSize GLWidget::sizeHint() const
{
    return QSize(width, height);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    sdkCreateTimer(&timer);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GLWidget::cleanup()
{
    sdkDeleteTimer(&timer);
	for (auto renderer:renderers)
		renderer.second->cleanup();
#if USE_PBO
    if (pbo) {
        glDeleteBuffers(1, &pbo);
        glDeleteTextures(1, &tex);
    }

    if(cuda_pbo_resource)
        cudaGraphicsUnregisterResource(cuda_pbo_resource);
#endif
}

void GLWidget::computeFPS()
{
    frameCount++;
    fpsCount++;
    if (fpsCount == fpsLimit)
    {
        float ifps = 1.f / (sdkGetAverageTimerValue(&timer) / 1000.f);
        qDebug() << "FPS: "<<ifps;
        fpsCount = 0;
//        fpsLimit = (int)MAX(1.f, ifps);
        sdkResetTimer(&timer);
    }
}

void GLWidget::TimerStart()
{
    sdkStartTimer(&timer);
}

void GLWidget::TimerEnd()
{
    sdkStopTimer(&timer);
    computeFPS();
}


void GLWidget::paintGL() {

	//makeCurrent();
    /****transform the view direction*****/
    TimerStart();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(transVec[0], transVec[1], transVec[2]);
    glMultMatrixf(transRot.data());
	glScalef(transScale * currentTransScale, transScale* currentTransScale, transScale* currentTransScale);

    glEnable(GL_DEPTH_TEST);
 //   int dataDim[3];
	//for (auto renderer : renderers)
	//	renderer->GetDataDim(dataDim[0], dataDim[1], dataDim[2]);
 //   int maxDim = std::max(std::max(dataDim[0], dataDim[1]), dataDim[2]);
	//float3 dataMin = ((TexPlaneRenderable*)renderers["slice0"])->GetDataMin();
	//float3 dataMax = ((TexPlaneRenderable*)renderers["slice0"])->GetDataMax();

	//float3 dataCenter = (dataMin + dataMax) * 0.5;
	//for the brain data, the center is always (0,0,0)
	float3 dataCenter = (dataMin + dataMax) * 0.5;
	float3 dataWidth = dataMax - dataMin;
    float dataMaxWidth = std::max(std::max(dataWidth.x, dataWidth.y), dataWidth.z);

	float scale = 2.0f / dataMaxWidth;
    glScalef(scale, scale, scale);
	glTranslatef(-dataCenter.x, -dataCenter.y, -dataCenter.z);

    GLfloat modelview[16];
    GLfloat projection[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    glGetFloatv(GL_PROJECTION_MATRIX, projection);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto renderer : renderers)
		renderer.second->draw(modelview, projection);

#if USE_PBO
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);


    // display results
    glClear(GL_COLOR_BUFFER_BIT);

    // draw image from PBO
    glDisable(GL_DEPTH_TEST);

    // copy from pbo to texture

    glBindTexture(GL_TEXTURE_2D, tex);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

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

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#elif DRAW_PIXELS
    uint* img = renderer->GetHostImage();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(width, height, GL_RGBA, GL_FLOAT, img);
#endif
    TimerEnd();
}

void Perspective(float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    xmax = ymax * aspectRatio;
    glFrustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

void GLWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
	for (auto renderer : renderers)
		renderer.second->resize(w, h);


    if(!initialized) {
		//GLint GlewInitResult = glewInit();
		//if (GLEW_OK != GlewInitResult)
		//{
		//	printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		//	exit(EXIT_FAILURE);
		//}
#if USE_PBO
        if (pbo)
        {
            // delete old buffer
            glDeleteBuffers(1, &pbo);
            glDeleteTextures(1, &tex);
        }


        // create pixel buffer object for display
        glGenBuffers(1, &pbo);
        glBindBuffer(GL_ARRAY_BUFFER, pbo);
        glBufferData(GL_ARRAY_BUFFER, width*height*sizeof(GLfloat)*4, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        renderer->SetPBO(pbo);


        // create texture for display
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        // register this buffer object with CUDA
        checkCudaErrors(cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, cudaGraphicsRegisterFlagsNone));
#endif
        //make init here because the window size is not updated in InitiateGL()
		makeCurrent();
		//ShaderProgram* glProg = new ShaderProgram();

		for (auto renderer:renderers)
			renderer.second->init();
        initialized = true;
    } else {
#if USE_PBO
        glBindBuffer(GL_ARRAY_BUFFER, pbo);
        glBufferData(GL_ARRAY_BUFFER, width * height*sizeof(GLfloat)*4, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
#endif
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	Perspective(30, (float)width / height, (float)0.1, (float)10e4);
//    glOrtho(-1.5,1.5,-1.5,1.5,-1.5,1.5);

    glMatrixMode(GL_MODELVIEW);
	
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (pinching)
		return;

	//if (oneFingerTimer.elapsed() < 100)
	//	return;

	QPointF pos = event->pos();
	//QPoint posGL = pixelPosToGLPos(event->pos());

	//for (auto renderer : renderers)
	//	renderer.second->mouseMove(posGL.x(), posGL.y(), QApplication::keyboardModifiers());

	//if (INTERACT_MODE::TRANSFORMATION != _interactMode)
	//	return;

	if ((event->buttons() & Qt::LeftButton) && (!pinched)) {
        QPointF from = pixelPosToViewPos(prevPos);
		QPointF to = pixelPosToViewPos(pos);
        *rot = trackball->rotate(from.x(), from.y(),
                                to.x(), to.y());
        float m[16];
        rot->matrix(m);
        QMatrix4x4 qm = QMatrix4x4(m).transposed();
        transRot = qm * transRot;

    } else if (event->buttons() & Qt::RightButton) {
        QPointF diff = pixelPosToViewPos(pos) - pixelPosToViewPos(prevPos);
        transVec[0] += diff.x();
        transVec[1] += diff.y();
    }
    prevPos = pos;
	//for (auto renderer : renderers)
	//	renderer.second->CameraChange();
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (pinching)
		return;

	//oneFingerTimer.restart();

	QPointF pos = event->pos();
	QPoint posGL = pixelPosToGLPos(event->pos());

	for (auto renderer : renderers)
		renderer.second->mousePress(posGL.x(), posGL.y(), QApplication::keyboardModifiers());

    prevPos = pos;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (pinching)
		return;

	pinched = false;

	QPoint posGL = pixelPosToGLPos(event->pos());
	for (auto renderer : renderers)
		renderer.second->mouseRelease(posGL.x(), posGL.y(), QApplication::keyboardModifiers());
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
	//if (INTERACT_MODE::TRANSFORMATION == _interactMode) {
		transScale *= exp(event->delta() * -0.003);
		update();
	//}
	for (auto renderer : renderers)
		renderer.second->MouseWheel(event->delta());
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
	//event->key;
	//((LineRenderable*)renderers["streamlines"])->GenGlyphAlongLine(0);
	//((LineRenderable*)renderers["streamlines"])->GenGlyphAlongLine(1);
}

bool GLWidget::event(QEvent *event)
{
	if (event->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent*>(event));
	return QWidget::event(event);
}

//http://doc.qt.io/qt-5/gestures-overview.html
bool GLWidget::gestureEvent(QGestureEvent *event)
{
//	qCDebug(lcExample) << "gestureEvent():" << event;
	//if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
	//	swipeTriggered(static_cast<QSwipeGesture *>(swipe));
	//else 
	//if (QGesture *pan = event->gesture(Qt::PanGesture))
	//	panTriggered(static_cast<QPanGesture *>(pan));
	if (QGesture *pinch = event->gesture(Qt::PinchGesture))
		pinchTriggered(static_cast<QPinchGesture *>(pinch));
	return true;
}

void GLWidget::pinchTriggered(QPinchGesture *gesture)
{
	if (!pinching) {
		pinching = true;
		pinched = true;
//		((GLLensRenderable*)GetRenderable("lens"))->RemoveAllLenses();
	}
	QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
	if (changeFlags & QPinchGesture::ScaleFactorChanged) {
		// transform only when there is no lens
		//if (INTERACT_MODE::TRANSFORMATION == _interactMode){
		//if (0 == ((GLLensRenderable*)GetRenderable("lens"))->GetNumCurtains()){
			currentTransScale = gesture->totalScaleFactor();// exp(/*event->delta()*/gesture->totalScaleFactor() * 0.01);
			update();
		}
		else {
			for (auto renderer : renderers)
				renderer.second->PinchScaleFactorChanged(gesture->totalScaleFactor());
		//}
	}
	if (changeFlags & QPinchGesture::CenterPointChanged) {
		// transform only when there is no lens
		//if (INTERACT_MODE::TRANSFORMATION == _interactMode){
			//if (0 == ((GLLensRenderable*)GetRenderable("lens"))->GetNumCurtains()){
			QPointF diff = pixelPosToViewPos(gesture->centerPoint())
				- pixelPosToViewPos(gesture->lastCenterPoint());
			transVec[0] += diff.x();
			transVec[1] += diff.y();
			update();
		//}
	}

	if (gesture->state() == Qt::GestureFinished) {
		transScale *= currentTransScale;
		currentTransScale = 1;
		pinching = false;
		//oneFingerTimer.restart();
	}
}

QPointF GLWidget::pixelPosToViewPos(const QPointF& p)
{
    return QPointF(2.0 * float(p.x()) / width - 1.0,
                   1.0 - 2.0 * float(p.y()) / height);
}

QPoint GLWidget::pixelPosToGLPos(const QPoint& p)
{
	return QPoint(p.x(), height - 1 - p.y());
}


//void GLWidget::AddLens()
//{
//	_interactMode = INTERACT_MODE::ADD_NODE;
//	((GLLensRenderable*)GetRenderable("lens"))->AddLens();
//}

//void GLWidget::AddLensNode()
//{
//	if (((GLLensRenderable*)GetRenderable("lens"))->GetNumCurtains() <= 0)
//		return;
//
//	_interactMode = INTERACT_MODE::ADD_NODE;
//}

Renderable* GLWidget::GetRenderable(const char* name)
{
	if (renderers.find(name) == renderers.end()) {
		std::cout << "No renderer named : " << name << std::endl;
		exit(1);
	}
	return renderers[name];
}

//void GLWidget::SetInteractModeToTransform()
//{ 
//	_interactMode = INTERACT_MODE::TRANSFORMATION; 
//	//((GLLensRenderable*)GetRenderable("lens"))->RemoveAllLenses();
//}

void GLWidget::SetVol(int3 dim)
{ 
	dataMin = make_float3(0, 0, 0);
	dataMax = make_float3(dim.x - 1, dim.y - 1, dim.z - 1);
}

void GLWidget::UpdateGL()
{
	update();
}