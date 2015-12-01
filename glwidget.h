#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QtWidgets>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <vector_types.h>
#include <vector_functions.h>

class Trackball;
class Rotation;
class StopWatchInterface;
class Renderable;

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

	void SetVol(int3 dim);

	void UpdateGL();

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

private:
    void computeFPS();

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

	//CHANGE_Huijie
	GLuint framebuffer, renderbuffer[2];
	bool picking = false;
	int xMouse, yMouse;
	int pickID;

    /****timing****/
    StopWatchInterface *timer = 0;
    int m_frame;
    int fpsCount = 0;        // FPS count for averaging
    int fpsLimit = 16;        // FPS limit for sampling
    int g_Index = 0;
    unsigned int frameCount = 0;

    std::map<std::string,Renderable*> renderers;

    bool initialized = false;
	bool pinching = false;
	//mark whether there is any pinching gesture in this sequence of gestures.
	// in order to prevent rotation if pinching is finished while one finger is still on the touch screen.
	bool pinched = false;	

	float3 dataMin = make_float3(0, 0, 0);
	float3 dataMax = make_float3(10, 10, 10);
};

#endif //GL_WIDGET_H
