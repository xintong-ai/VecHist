#include "GL2DProjWidget.h"
#include <vector_types.h>
#include <vector_functions.h>
#include <GLTextureCube.h>

#define qgl	QOpenGLContext::currentContext()->functions()
#include "ShaderProgram.h"

//2D projection of cube map:
//http://paulbourke.net/geometry/transformationprojection/

GL2DProjWidget::GL2DProjWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	//texPlaneTracer = r;
	//r->SetSlice2DWidget(this);
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
	return QSize(256, 256);
}

void GL2DProjWidget::loadShaders()
{
#define GLSL(shader) "#version 440\n" #shader
	const char* slice2DVS =
		GLSL(
	layout(location = 0) in vec3 vertexPosition;
	//layout(location = 1) in vec2 vertexUV;

	smooth out vec2 UV;

	//uniform vec2 texSizes;
	uniform vec2 winSizes;


	void main(){

		//float texSizeMax;
		//texSizeMax = texSizes.x > texSizes.y? texSizes.x:texSizes.y;
		//vec2 texSizeRatio = vec2(texSizes.x / texSizeMax * 0.0001 + 1, texSizes.y / texSizeMax * 0.0001 + 1);

		float winSizeMax = winSizes.x > winSizes.y ? winSizes.x : winSizes.y;
		vec2 winSizeRatio = vec2(winSizes.y / winSizeMax, winSizes.x / winSizeMax);

		gl_Position = vec4(vertexPosition.x  * winSizeRatio.x, vertexPosition.y * winSizeRatio.y, vertexPosition.z, 1);
		UV = vertexPosition.xy;
	}

	);

	//the texture only returns vec4, so we should change the texture input format
	const char* slice2DFS =
		GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
	#define PI 3.141592653589793238462643383279 \n
	// Interpolated values from the vertex shaders
	smooth in vec2 UV;

	// Ouput data
	out vec3 color;

	// Values that stay constant for the whole mesh.
	//uniform sampler2D myTextureSampler;
	uniform samplerCube env;

	void main(){
		//color = texture(myTextureSampler, UV).rgb;// *0.000001 + vec3(UV.x, UV.y, 0);
		float theta = UV.x * PI;
		float phi = UV.y * PI * 0.5;

		vec3 norm = vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));

		float v = textureCube(env, norm).x;
		//vec3 unlitColor = GetColor2(norm, v);
		//vec3 unlitColor = GetColor(v, 0, 1).xyz;// vec3(v, 0, 0); //vec4((normal.x + 1) * 0.5, (normal.y + 1), (normal.z + 1) * 0.5, 1.0f);
		color = vec3(v, v, v);
	}
	);


	glProg = new ShaderProgram();
	glProg->initFromStrings(slice2DVS, slice2DFS);

	glProg->addAttribute("vertexPosition");
	//glProg->addAttribute("vertexUV");

	//glProg->addUniform("texSizes");
	glProg->addUniform("winSizes");
	glProg->addUniform("env");
	
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

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	//glVertexAttribPointer(glProg->attribute("vertexUV"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 2, texCoords, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glEnableVertexAttribArray(glProg->attribute("vertexUV"));

	m_vao->release();
}

void GL2DProjWidget::paintGL() {
	//if (textureID > 100)
	//	return;
	if (nullptr == tex)
		return;

	glProg->use();
	m_vao->bind();

	//glUniform2f(glProg->uniform("texSizes"), texSz1, texSz2);
	glUniform2f(glProg->uniform("winSizes"), winWidth, winHeight);
	glUniform1i(glProg->uniform("env"), GLint(0));

	//glBindTexture(GL_TEXTURE_2D, textureID);
	tex->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	tex->unbind();
	m_vao->release();
	glProg->disable();
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

void GL2DProjWidget::SlotSetCubeTexture(GLTextureCube* v)
{ 
	tex = v; 
	update();
}
