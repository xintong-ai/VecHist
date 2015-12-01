#ifndef GL_2D_PROJ_WIDGET_H
#define GL_2D_PROJ_WIDGET_H

#include <QtWidgets>
#include <QOpenGLWidget>
//CHANGE_Huijie
#include "vector_types.h"
#include "vector_functions.h"

class ShaderProgram;
class GLTextureCube;
class GLTexture2D;
struct Cube;
class GL2DProjWidget :public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit GL2DProjWidget(QWidget *parent = 0);
	~GL2DProjWidget();
private:

	void loadShaders();
	QOpenGLVertexArrayObject* m_vao;

	unsigned int vbo_vert;

	int winWidth = 0, winHeight = 0;

	GLTextureCube* tex = nullptr;
	GLTexture2D* tex2d = nullptr;

	int type = 1;

	//CHANGE_Huijie
	int cubeSize = 0;
	float cubeMax, cubeMin;
	int texMode = 0;
	int lineVerNum = 0;
	float3 *cubeColor;
<<<<<<< HEAD
	QOpenGLVertexArrayObject* m_vao2;
	unsigned int vbo_vert2;

	QLabel *label1;
	QLabel *label2;
	QLabel *label3;
	QLabel *label4;
	QLabel *label5;
	QLabel *label6;
	QLabel *label7;
	QLabel *label8;
	QLabel *label9;
	QLabel *label10;
	QLabel *label11;
	QLabel *label12;
	QLabel *label13;
=======
>>>>>>> origin/master

protected:

	virtual void initializeGL() Q_DECL_OVERRIDE;
	virtual void paintGL() Q_DECL_OVERRIDE;
	virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
	QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	QSize sizeHint() const Q_DECL_OVERRIDE;
	int heightForWidth(int w) const;

	ShaderProgram *glProg;
	ShaderProgram *glProg6Faces;
	//CHANGE_Huijie
	ShaderProgram *glProgGrid;

	public slots:
	void SlotSetCubeTexture(GLTextureCube* v, Cube* c);// { tex = v; }
};
#endif //GL_2D_PROJ_WIDGET_H