#ifndef GL_2D_PROJ_WIDGET_H
#define GL_2D_PROJ_WIDGET_H

#include <QtWidgets>
#include <QOpenGLWidget>

class ShaderProgram;
class GLTextureCube;
struct Cube;
class GL2DProjWidget :public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit GL2DProjWidget(QWidget *parent = 0);
	~GL2DProjWidget();

	//void SetTextureID(GLuint id) { textureID = id; }

	void SetTextureSize(int size) { texSz = size; }

private:

	void loadShaders();

	//GLuint textureID = 999;
	QOpenGLVertexArrayObject* m_vao;

	unsigned int vbo_vert;
	unsigned int vbo_uv;

	int texSz = 0;

	int winWidth = 0, winHeight = 0;

	GLTextureCube* tex = nullptr;

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

public slots:
	void SlotSetCubeTexture(GLTextureCube* v, Cube* c);// { tex = v; }
};
#endif //GL_2D_PROJ_WIDGET_H
