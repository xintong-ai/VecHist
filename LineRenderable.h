#ifndef GL_Line_TRACER_H
#define GL_Line_TRACER_H
#include "Renderable.h"
class ShaderProgram;
class QOpenGLVertexArrayObject;
class Streamline;
#include <QObject>

class LineRenderable : public Renderable, public QObject
{
	Q_OBJECT
public:
	LineRenderable(Streamline* r);

    void init() override;

	void UpdateData() override;

    void resize(int width, int height) override;

    void draw(float modelview[16], float projection[16]) override;

    void cleanup() override;

	virtual void mousePress(int x, int y, int modifier) override;
	virtual void mouseRelease(int x, int y, int modifier) override;
	virtual void mouseMove(int x, int y, int modifier) override;

	void SetLineGenerator(Streamline* v) { lineGenerator = v; }
	void GenGlyphAlongLine(int i);

private:

    void loadShaders();

	QOpenGLVertexArrayObject* m_vao;

	unsigned int vbo_vert;
	unsigned int vbo_color;

	float transSize = 0.5;

	std::vector<std::vector<float4> > lines;

	std::vector<int> lineOffsets;
	//std::vector<int> _primitiveLengths;

protected:

    ShaderProgram *glProg;
	Streamline* lineGenerator;

signals:
	void SigGenCubeAlongLine(float4* line, int nv);
	void SigTest();
};
#endif //GL_Line_TRACER_H
