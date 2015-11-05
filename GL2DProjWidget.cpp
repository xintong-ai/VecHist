#include "GL2DProjWidget.h"
#include <vector_types.h>
#include <vector_functions.h>
#include <GLTextureCube.h>
#include "Cubemap.h"

#define qgl	QOpenGLContext::currentContext()->functions()
#include "ShaderProgram.h"
#include <memory>
//2D projection of cube map:
//http://paulbourke.net/geometry/transformationprojection/

GL2DProjWidget::GL2DProjWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	//texPlaneTracer = r;
	//r->SetSlice2DWidget(this);
	QSizePolicy p(sizePolicy());
	p.setHeightForWidth(true);
	setSizePolicy(p);

}

GL2DProjWidget::~GL2DProjWidget()
{
}

QSize GL2DProjWidget::minimumSizeHint() const
{
	return QSize(128, 128);
}

QSize GL2DProjWidget::sizeHint() const
{
	return QSize(256, heightForWidth(256));
}

void GL2DProjWidget::loadShaders()
{
#define GLSL(shader) "#version 440\n" #shader
	const char* slice2DVS =
		GLSL(
	layout(location = 0) in vec3 vertexPosition;
	smooth out vec2 UV;
	void main(){
		gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
		UV = vertexPosition.xy;
	}
	);

	//the texture only returns vec4, so we should change the texture input format
	const char* slice2DFS =
		GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
	#define PI 3.141592653589793238462643383279 \n
	smooth in vec2 UV;
	out vec3 color;
	uniform samplerCube env;
	void main(){
		float theta = UV.x * PI;
		float phi = UV.y * PI * 0.5;
		vec3 norm = vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));
		float v = textureCube(env, norm).x;
		color = vec3(v, v, v);
	}
	);

	const char* slice2D6FacesVS =
		GLSL(
	layout(location = 0) in vec3 vertexPosition;
	smooth out vec2 UV;
	void main(){
		gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
		UV = (vertexPosition.xy + 1) * 0.5;
	}
	);

	const char* slice2D6FacesFS =
		GLSL(
	smooth in vec2 UV;
	out vec3 color;
	uniform sampler2D tex;
	void main(){
		color = texture(tex, UV).rgb;// *0.000001 + vec3(UV.x, UV.y, 0);
	}
	);


	glProg = new ShaderProgram();
	glProg->initFromStrings(slice2DVS, slice2DFS);
	glProg->addAttribute("vertexPosition");
	glProg->addUniform("env");
	
	glProg6Faces = new ShaderProgram();
	glProg6Faces->initFromStrings(slice2D6FacesVS, slice2D6FacesFS);
	glProg6Faces->addAttribute("vertexPosition");
	glProg6Faces->addUniform("tex");
	
}

void GL2DProjWidget::initializeGL()
{
	initializeOpenGLFunctions();


	float3 corners[] = { 
		make_float3(-1, -1, 0),
		make_float3(1, -1, 0),
		make_float3(1, 1, 0),
		make_float3(-1, 1, 0)
	};

	//float texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();
	glGenBuffers(1, &vbo_vert);
	//glGenBuffers(1, &vbo_uv);

	loadShaders();
	
	m_vao->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	glVertexAttribPointer(glProg->attribute("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &corners[0].x, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(glProg->attribute("vertexPosition"));

	m_vao->release();
}

void GL2DProjWidget::paintGL() {
	//if (textureID > 100)
	//	return;


	m_vao->bind();

	if (0 == type) {
		if (nullptr == tex2d)
			return;
		glProg6Faces->use();
		glUniform1i(glProg6Faces->uniform("tex"), GLint(0));
		tex2d->bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		tex2d->unbind();
		glProg6Faces->disable();
	}
	else {
		if (nullptr == tex)
			return;
		glProg->use();
		//glUniform2f(glProg->uniform("texSizes"), texSz1, texSz2);
		glUniform1i(glProg->uniform("env"), GLint(0));

		//glBindTexture(GL_TEXTURE_2D, textureID);
		tex->bind();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		tex->unbind();
		glProg->disable();
	}
	m_vao->release();
}


void GL2DProjWidget::resizeGL(int w, int h)
{
	winWidth = w;
	winHeight = h;
}

void GL2DProjWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void GL2DProjWidget::mousePressEvent(QMouseEvent *event)
{
	if (0 == type)
		type = 1;
	else
		type = 0;
	update();
}

void GL2DProjWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void GL2DProjWidget::wheelEvent(QWheelEvent * event)
{
}

void GL2DProjWidget::keyPressEvent(QKeyEvent * event)
{

}

void GL2DProjWidget::SlotSetCubeTexture(GLTextureCube* v, Cube* c)
{ 
	tex = v; 
	//std::unique_ptr<float[]> hist(new float[c->cubemap_size * c->cubemap_size * 12], );
	int sz = c->cubemap_size;
	std::vector<float> img(sz * sz * 12, 0.0f);
	/*for (int i = 0; i < sz * sz * 12; i++) {
		img[] = i / (sz * sz * 12);
	}*/

	for (int j = 0; j < sz; j++) {
		for (int i = 0; i < sz; i++) {
			img[sz * 4 * sz + i + j * 4 * sz] = c->data[sz * sz + j * sz + i];
			img[sz * 4 * sz + sz + i + j * 4 * sz] = c->data[5 * sz * sz + j * sz + i]; 
			img[sz * 4 * sz + sz * 2 + i + j * 4 * sz] = c->data[j * sz + i]; 
			img[sz * 4 * sz + sz * 3 + i + j * 4 * sz] = c->data[4 * sz * sz + j * sz + i];

			img[sz * 8 * sz + sz + i + j * 4 * sz] = c->data[2 * sz * sz + j * sz + i];
			img[sz + i + j * 4 * sz] = c->data[3 * sz * sz + j * sz + i];
		}
	}
	if (nullptr == tex2d) {
		tex2d = new GLTexture2D(sz * 4, sz * 3);
	}
	tex2d->load(sz * 4, sz * 3, &img[0]);
	update();
}

int GL2DProjWidget::heightForWidth(int w) const { 
	return w * 0.5; 
}