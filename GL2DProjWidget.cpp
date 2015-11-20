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
	QSizePolicy p(sizePolicy());
	p.setHeightForWidth(true);
	setSizePolicy(p);

	//CHANGE_Huijie
	ReadColormap();
}

//CHANGE_Huijie
void GL2DProjWidget::ReadColormap()
{
	std::ifstream fin;
	fin.open("ColorMap1.txt", std::ios::in);
	if (!fin.is_open()){
		std::cerr << "Error: unable to read the input colormap file."
			<< std::endl;
	}

	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 3; ++j){
			fin >> colormapRed[i][j];
			colormapRed[i][j] = 1.0 * colormapRed[i][j] / 255;
			//std::cout << colormapRed[i][j] << " ";
		}
	}

	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 3; ++j){
			fin >> colormapGreen[i][j];
			colormapGreen[i][j] = 1.0 * colormapGreen[i][j] / 255;
			//std::cout << colormapGreen[i][j] << " ";
		}
	}

	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 3; ++j){
			fin >> colormapBlue[i][j];
			colormapBlue[i][j] = 1.0 * colormapBlue[i][j] / 255;
			//std::cout << colormapBlue[i][j] << " ";
		}
	}

	fin.close();
	fin.clear();
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
	//CHANGE_Huijie
	uniform float cmax;
	uniform float cmin;
	uniform vec3 cmRed[10];
	uniform vec3 cmGreen[10];
	uniform vec3 cmBlue[10];

	void main(){
		float theta = UV.x * PI;
		float phi = UV.y * PI * 0.5;
		vec3 norm = vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));
		//float v = textureCube(env, norm).x * 10;
		//color = vec3(v, v, v);

		//CHANGE_Huijie
		float v = textureCube(env, norm).x;
		float frac = 0.6;
		float pdv = (1 - frac) * (cmax - cmin) / 8.0;
		color = vec3(0.8, 0.8, 0.8);
		if (v < 0.000001)
			color = vec3(0.8f, 0.8f, 0.8f);
		else if (abs(norm.x) >= abs(norm.y) && abs(norm.x) >= abs(norm.z)){
			if (v >= cmax - frac*(cmax - cmin)){
				color.r = cmRed[8].r + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmRed[9].r - cmRed[8].r);
				color.g = cmRed[8].g + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmRed[9].g - cmRed[8].g);
				color.b = cmRed[8].b + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmRed[9].b - cmRed[8].b);
			}
			else{
				int i = 0;
				for (i = 0; i < 8; i++) {
					if (v >= (cmin + i*pdv) && v < (cmin + (i + 1)*pdv)) {
						color.r = cmRed[i].r + (v - cmin - i*pdv) / pdv * (cmRed[i + 1].r - cmRed[i].r);
						color.g = cmRed[i].g + (v - cmin - i*pdv) / pdv * (cmRed[i + 1].g - cmRed[i].g);
						color.b = cmRed[i].b + (v - cmin - i*pdv) / pdv * (cmRed[i + 1].b - cmRed[i].b);
						break;
					}
				}
			}
		}
		else if (abs(norm.y) >= abs(norm.x) && abs(norm.y) >= abs(norm.z)){
			if (v >= cmax - frac*(cmax - cmin)){
				color.r = cmGreen[8].r + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmGreen[9].r - cmGreen[8].r);
				color.g = cmGreen[8].g + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmGreen[9].g - cmGreen[8].g);
				color.b = cmGreen[8].b + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmGreen[9].b - cmGreen[8].b);
			}
			else{
				int i = 0;
				for (i = 0; i < 8; i++) {
					if (v >= (cmin + i*pdv) && v < (cmin + (i + 1)*pdv)) {
						color.r = cmGreen[i].r + (v - cmin - i*pdv) / pdv * (cmGreen[i + 1].r - cmGreen[i].r);
						color.g = cmGreen[i].g + (v - cmin - i*pdv) / pdv * (cmGreen[i + 1].g - cmGreen[i].g);
						color.b = cmGreen[i].b + (v - cmin - i*pdv) / pdv * (cmGreen[i + 1].b - cmGreen[i].b);
						break;
					}
				}
			}
		}
		else if (abs(norm.z) >= abs(norm.x) && abs(norm.z) >= abs(norm.y)){
			if (v >= cmax - frac*(cmax - cmin)){
				color.r = cmBlue[8].r + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmBlue[9].r - cmBlue[8].r);
				color.g = cmBlue[8].g + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmBlue[9].g - cmBlue[8].g);
				color.b = cmBlue[8].b + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmBlue[9].b - cmBlue[8].b);
			}
			else{
				int i = 0;
				for (i = 0; i < 8; i++) {
					if (v >= (cmin + i*pdv) && v < (cmin + (i + 1)*pdv)) {
						color.r = cmBlue[i].r + (v - cmin - i*pdv) / pdv * (cmBlue[i + 1].r - cmBlue[i].r);
						color.g = cmBlue[i].g + (v - cmin - i*pdv) / pdv * (cmBlue[i + 1].g - cmBlue[i].g);
						color.b = cmBlue[i].b + (v - cmin - i*pdv) / pdv * (cmBlue[i + 1].b - cmBlue[i].b);
						break;
					}
				}
			}
		}
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
	//CHANGE_Huijie
	uniform float cmax;
	uniform float cmin;
	uniform vec3 cmRed[10];
	uniform vec3 cmGreen[10];
	uniform vec3 cmBlue[10];

	void main(){
		//color = texture(tex, UV).rgb * 10;// *0.000001 + vec3(UV.x, UV.y, 0);

		//CHANGE_Huijie
		color = vec3(0.8, 0.8, 0.8);
		int sca = 50;
		float frac = 0.6;
		float v = texture(tex, UV).x;
		float pdv = (1 - frac) * (cmax - cmin) / 8.0;
		if (v < 0.000001)
			color = vec3(0.8f, 0.8f, 0.8f);
		else if (((UV.x >= 0.25 && UV.x <= 0.5) && (UV.y >= 0 && UV.y <= 1.0 / 3.0)) ||
			((UV.x >= 0.25 && UV.x <= 0.5) && (UV.y >= 2.0 / 3.0 && UV.y <= 1.0))){
			if (v >= cmax - frac*(cmax - cmin)){
				color.r = cmGreen[8].r + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmGreen[9].r - cmGreen[8].r);
				color.g = cmGreen[8].g + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmGreen[9].g - cmGreen[8].g);
				color.b = cmGreen[8].b + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmGreen[9].b - cmGreen[8].b);
			}
			else{
				int i = 0;
				for (i = 0; i < 8; i++) {
					if (v >= (cmin + i*pdv) && v < (cmin + (i + 1)*pdv)) {
						color.r = cmGreen[i].r + (v - cmin - i*pdv) / pdv * (cmGreen[i + 1].r - cmGreen[i].r);
						color.g = cmGreen[i].g + (v - cmin - i*pdv) / pdv * (cmGreen[i + 1].g - cmGreen[i].g);
						color.b = cmGreen[i].b + (v - cmin - i*pdv) / pdv * (cmGreen[i + 1].b - cmGreen[i].b);
						break;
					}
				}
			}
		}
		else if (((UV.x >= 0 && UV.x <= 0.25) && (UV.y >= 1.0 / 3.0 && UV.y <= 2.0 / 3.0)) ||
			((UV.x >= 0.5 && UV.x <= 0.75) && (UV.y >= 1.0 / 3.0 && UV.y <= 2.0 / 3.0))){
			if (v >= cmax - frac*(cmax - cmin)){
				color.r = cmRed[8].r + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmRed[9].r - cmRed[8].r);
				color.g = cmRed[8].g + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmRed[9].g - cmRed[8].g);
				color.b = cmRed[8].b + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmRed[9].b - cmRed[8].b);
			}
			else{
				int i = 0;
				for (i = 0; i < 8; i++) {
					if (v >= (cmin + i*pdv) && v < (cmin + (i + 1)*pdv)) {
						color.r = cmRed[i].r + (v - cmin - i*pdv) / pdv * (cmRed[i + 1].r - cmRed[i].r);
						color.g = cmRed[i].g + (v - cmin - i*pdv) / pdv * (cmRed[i + 1].g - cmRed[i].g);
						color.b = cmRed[i].b + (v - cmin - i*pdv) / pdv * (cmRed[i + 1].b - cmRed[i].b);
						break;
					}
				}
			}
		}
		else if (((UV.x >= 0.25 && UV.x <= 0.5) && (UV.y >= 1.0 / 3.0 && UV.y <= 2.0 / 3.0)) ||
			((UV.x >= 0.75 && UV.x <= 1.0) && (UV.y >= 1.0 / 3.0 && UV.y <= 2.0 / 3.0))){
			if (v >= cmax - frac*(cmax - cmin)){
				color.r = cmBlue[8].r + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmBlue[9].r - cmBlue[8].r);
				color.g = cmBlue[8].g + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmBlue[9].g - cmBlue[8].g);
				color.b = cmBlue[8].b + (v - cmin - (1 - frac)*(cmax - cmin)) / (frac*(cmax - cmin)) * (cmBlue[9].b - cmBlue[8].b);
			}
			else{
				int i = 0;
				for (i = 0; i < 8; i++) {
					if (v >= (cmin + i*pdv) && v < (cmin + (i + 1)*pdv)) {
						color.r = cmBlue[i].r + (v - cmin - i*pdv) / pdv * (cmBlue[i + 1].r - cmBlue[i].r);
						color.g = cmBlue[i].g + (v - cmin - i*pdv) / pdv * (cmBlue[i + 1].g - cmBlue[i].g);
						color.b = cmBlue[i].b + (v - cmin - i*pdv) / pdv * (cmBlue[i + 1].b - cmBlue[i].b);
						break;
					}
				}
			}
		}
	}
	);

	//CHANGE_Huijie
	glProg = new ShaderProgram();
	glProg->initFromStrings(slice2DVS, slice2DFS);
	glProg->addAttribute("vertexPosition");
	glProg->addUniform("env");
	glProg->addUniform("cmax");
	glProg->addUniform("cmin");
	glProg->addUniform("cmRed");
	glProg->addUniform("cmGreen");
	glProg->addUniform("cmBlue");
	//glProg->addUniform("cubesize");

	glProg6Faces = new ShaderProgram();
	glProg6Faces->initFromStrings(slice2D6FacesVS, slice2D6FacesFS);
	glProg6Faces->addAttribute("vertexPosition");
	glProg6Faces->addUniform("tex");
	glProg6Faces->addUniform("cmax");
	glProg6Faces->addUniform("cmin");
	glProg6Faces->addUniform("cmRed");
	glProg6Faces->addUniform("cmGreen");
	glProg6Faces->addUniform("cmBlue");
	
}

void GL2DProjWidget::initializeGL()
{
	initializeOpenGLFunctions();
	loadShaders();
	float3 corners[] = {
		make_float3(-1, -1, 0),
		make_float3(1, -1, 0),
		make_float3(1, 1, 0),
		make_float3(-1, 1, 0)
	};

	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();
	glGenBuffers(1, &vbo_vert);
	m_vao->bind();
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	glVertexAttribPointer(glProg->attribute("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &corners[0].x, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(glProg->attribute("vertexPosition"));
	m_vao->release();
}

void GL2DProjWidget::paintGL() {
	//CHANGE_Huijie
	glClear(GL_COLOR_BUFFER_BIT);
	if (0 == type) {
		if (nullptr == tex2d)
			return;
		m_vao->bind();
		glProg6Faces->use();
		glUniform1i(glProg6Faces->uniform("tex"), GLint(0));
		glUniform1f(glProg6Faces->uniform("cmax"), cubeMax);
		glUniform1f(glProg6Faces->uniform("cmin"), cubeMin);
		glUniform3fv(glProg6Faces->uniform("cmRed"), 10, (const GLfloat*)&colormapRed[0]);
		glUniform3fv(glProg6Faces->uniform("cmGreen"), 10, (const GLfloat*)&colormapGreen[0]);
		glUniform3fv(glProg6Faces->uniform("cmBlue"), 10, (const GLfloat*)&colormapBlue[0]);
		tex2d->bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		tex2d->unbind();
		glProg6Faces->disable();
		m_vao->release();

		glColor3f(1.0f, 1.0f, 1.0f);
		//glColor3f(0.0f, 0.0f, 0.0f);
		int sizeWidth = cubeSize * 4;
		for (int i = 0; i <= sizeWidth; ++i){
			if (i >= cubeSize && i <= 2 * cubeSize){
				/*if (i == cubeSize || i == 2 * cubeSize)
				glColor3f(0.0f, 0.0f, 0.0f);
				else
				glColor3f(1.0f, 1.0f, 1.0f);*/
				glBegin(GL_LINES);
				glVertex3f((2.0f * i / sizeWidth) - 1.0f, -1.0f, 0.0f);
				glVertex3f((2.0f * i / sizeWidth) - 1.0f, 1.0f, 0.0f);
				glEnd();
			}
			else{
				glBegin(GL_LINES);
				glVertex3f((2.0f * i / sizeWidth) - 1.0f, (1.0f / 3.0f) * 2.0f - 1.0f, 0.0f);
				glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f / 3.0f) * 2.0f - 1.0f, 0.0f);
				glEnd();
			}

		}
		int sizeHeight = cubeSize * 3;
		for (int i = 0; i <= sizeHeight; ++i){
			if (i >= cubeSize && i <= 2 * cubeSize){
				/*if (i == cubeSize || i == 2 * cubeSize)
				glColor3f(0.0f, 0.0f, 0.0f);
				else
				glColor3f(1.0f, 1.0f, 1.0f);*/
				glBegin(GL_LINES);
				glVertex3f(-1.0f, (2.0f * i / sizeHeight) - 1.0f, 0.0f);
				glVertex3f(1.0f, (2.0f * i / sizeHeight) - 1.0f, 0.0f);
				glEnd();
			}
			else{
				glBegin(GL_LINES);
				glVertex3f(-0.5f, (2.0f * i / sizeHeight) - 1.0f, 0.0f);
				glVertex3f(0.0f, (2.0f * i / sizeHeight) - 1.0f, 0.0f);
				glEnd();
			}
		}


	}
	else {
		if (nullptr == tex)
			return;
		m_vao->bind();
		glProg->use();
		glUniform1i(glProg->uniform("env"), GLint(0));
		glUniform1f(glProg->uniform("cmax"), cubeMax);
		glUniform1f(glProg->uniform("cmin"), cubeMin);
		glUniform3fv(glProg->uniform("cmRed"), 10, (const GLfloat*)&colormapRed[0]);
		glUniform3fv(glProg->uniform("cmGreen"), 10, (const GLfloat*)&colormapGreen[0]);
		glUniform3fv(glProg->uniform("cmBlue"), 10, (const GLfloat*)&colormapBlue[0]);
		//glUniform1i(glProg->uniform("cubesize"), cubeSize);
		tex->bind();

		if (texMode == 0){
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if (texMode == 1){
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		tex->unbind();
		glProg->disable();
		m_vao->release();

	}
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
	//CHANGE_Huijie
	if (event->button() == Qt::LeftButton){
		if (0 == type)
			type = 1;
		else
			type = 0;
	}

	if (event->button() == Qt::RightButton){
		if (texMode == 0)
			texMode = 1;
		else
			texMode = 0;
	}

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
	int sz = c->cubemap_size;
	std::vector<float> img(sz * sz * 12, 0.0f);

	/*for (int j = 0; j < sz; j++) {
		for (int i = 0; i < sz; i++) {
			img[sz * 4 * sz + i + j * 4 * sz] = c->data[sz * sz + j * sz + i];
			img[sz * 4 * sz + sz + i + j * 4 * sz] = c->data[5 * sz * sz + j * sz + i];
			img[sz * 4 * sz + sz * 2 + i + j * 4 * sz] = c->data[j * sz + i];
			img[sz * 4 * sz + sz * 3 + i + j * 4 * sz] = c->data[4 * sz * sz + j * sz + i];
			img[sz * 8 * sz + sz + i + j * 4 * sz] = c->data[2 * sz * sz + j * sz + i];
			img[sz + i + j * 4 * sz] = c->data[3 * sz * sz + j * sz + i];
		}
	}*/

	//CHANGE_Huijie
	cubeSize = c->cubemap_size;
	cubeMax = Cube::cubeMaxValue;
	cubeMin = Cube::cubeMinValue;
	for (int j = 0; j < sz; j++) {
		for (int i = 0; i < sz; i++) {
			img[sz * 4 * sz + i + j * 4 * sz] = c->data[sz * sz + j * sz + i];	//X- 
			img[sz * 4 * sz + sz + i + j * 4 * sz] = c->data[4 * sz * sz + j * sz + i];	//Z+ 
			img[sz * 4 * sz + sz * 2 + i + j * 4 * sz] = c->data[j * sz + i];	//X+ 
			img[sz * 4 * sz + sz * 3 + i + j * 4 * sz] = c->data[5 * sz * sz + j * sz + i];	//Z-
			img[sz * 8 * sz + sz + i + j * 4 * sz] = c->data[3 * sz * sz + j * sz + i];	//Y- 
			img[sz + i + j * 4 * sz] = c->data[2 * sz * sz + j * sz + i];	//Y+ 
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