#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QtWidgets>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLWidget>
//#include <defines.h>

class Trackball;
class Rotation;
class StopWatchInterface;
class DataMgr;
class Renderable;
class LineReader;

class GLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

	void AddRenderable(const char* name, void* r);

	Renderable* GetRenderable(const char* name);

    void GetWindowSize(int &w, int &h) {w = width; h = height;}

	//void SetDataMgr(DataMgr* v) { dataMgr = v; }

	//DEFORM_MODE GetDeformMode(){ return _deformMode; }
	//INTERACT_MODE GetInteractMode(){ return _interactMode; }
	//SOURCE_MODE GetSourceMode(){ return _sourceMode; }

	//void AddLens();
	//void AddLensNode();

	//void SetInteractModeToTransform();// { _interactMode = INTERACT_MODE::TRANSFORMATION; }
	//void SetInteractModeToModifyLens(){ _interactMode = INTERACT_MODE::MODIFY_LENS; }

	//void SetLineReader(LineReader* r) { lineReader = r; }

protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
	virtual bool event(QEvent *event) Q_DECL_OVERRIDE;

	uint width = 512, height = 512;

    //uint *d_output = NULL;

private:
    void computeFPS();

    void printModelView();

    void cleanup();

    void TimerStart();

    void TimerEnd();


    QPointF pixelPosToViewPos(const QPointF& p);

	QPoint pixelPosToGLPos(const QPoint& p);

	bool gestureEvent(QGestureEvent *event);

	void pinchTriggered(QPinchGesture *gesture);
		/*****view*****/
    //transformation states
    QVector3D transVec = QVector3D(0.0f, 0.0f, -5.0f);//move it towards the front of the camera
    QMatrix4x4 transRot;
	float transScale = 1;
	float currentTransScale = 1;

    Trackball *trackball;
    QPointF prevPos;//previous mouse position
    Rotation *rot;

    /****timing****/
    StopWatchInterface *timer = 0;
    int m_frame;
    int fpsCount = 0;        // FPS count for averaging
    int fpsLimit = 16;        // FPS limit for sampling
    int g_Index = 0;
    unsigned int frameCount = 0;

    std::map<std::string,Renderable*> renderers;

    //pbo
    //GLuint pbo = 0;     // OpenGL pixel buffer object
    //GLuint tex = 0;     // OpenGL texture object
    //struct cudaGraphicsResource *cuda_pbo_resource; // CUDA Graphics Resource (to transfer PBO)

    bool initialized = false;
	bool pinching = false;
	//mark whether there is any pinching gesture in this sequence of gestures.
	// in order to prevent rotation if pinching is finished while one finger is still on the touch screen.
	bool pinched = false;	

	//DataMgr* dataMgr;

	//DEFORM_MODE _deformMode = DEFORM_MODE::MODE_LINE;
	//SOURCE_MODE _sourceMode = SOURCE_MODE::MODE_LENS;
	//INTERACT_MODE _interactMode = INTERACT_MODE::TRANSFORMATION;
	//VISUAL_MODE _visualMode;

	//LineReader* lineReader;

	//QElapsedTimer oneFingerTimer;
	

	//bool doAnimating = true;
private slots:
     //void animate();
};

#endif //GL_WIDGET_H
