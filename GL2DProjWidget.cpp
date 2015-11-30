#include "GL2DProjWidget.h"
#include <vector_types.h>
#include <vector_functions.h>
#include <GLTextureCube.h>
#include "Cubemap.h"
//CHANGE_Huijie
#include <GL/freeglut.h>
#include "helper_math.h"// to define normalize()

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
	uniform int cubesize;

	void main(){
		float theta = UV.x * PI;
		float phi = UV.y * PI * 0.5;
		vec3 norm = vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta));
		/*float v = textureCube(env, norm).x * 10 + (cmax - cmin) * 0.0001;
		color = vec3(v, v, v);*/

		//CHANGE_Huijie
		color = vec3(0.0, 0.0, 0.0);
		float v = textureCube(env, norm).x;
		float c = (v - cmin) / (cmax - cmin);
		color = vec3(c, c, c);
		//color = vec3(0.8, 0.8, 0.8);

		if (abs(norm.x) > abs(norm.y) && abs(norm.x) > abs(norm.z)){
			double a = norm.y / abs(norm.x);
			double b = norm.z / abs(norm.x);
			double d = 2.0 / (cubesize-1);
			for (int i = 0; i < cubesize - 1; ++i){
				double err = 0.02;
				double offset = 0.0;
				if (cubesize % 2 == 1)
					offset = d * (0.1 + abs(1.0 * (cubesize) / 2 - 1 - i)*0.03);
				if (cubesize % 2 == 0)
					offset = d * (0.1 + abs(1.0 * (cubesize - 1) / 2 - i)*0.03);
				if (i == 0 || i == cubesize - 2) offset = d * 0.3;
				if (cubesize % 2 == 1)
					err = 0.02 + abs(1.0 * (cubesize) / 2 - 1 - i) * 0.0025;
				if (cubesize % 2 == 0)
					err = 0.02 + abs(1.0 * (cubesize-1) / 2 - i) * 0.0025;
				if (cubesize % 2 == 1 && i > 1.0 * (cubesize) / 2 - 1) offset = -offset;
				if (cubesize % 2 == 0 && i > 1.0 * (cubesize - 1) / 2) offset = -offset;
				if (cubesize % 2 == 0 && i == (cubesize / 2) - 1){
					err = 0.015;
					offset = 0;
				}
				if (a + 1.0 - (i + 0.5) * d - offset < err && a + 1.0 - (i + 0.5) * d - offset > -err ||
					b + 1.0 - (i + 0.5) * d - offset < err && b + 1.0 - (i + 0.5) * d - offset > -err){
					color = vec3(abs(norm.x), abs(norm.y), abs(norm.z));
					break;
				}
			}
		}
		else if (abs(norm.y) > abs(norm.x) && abs(norm.y) > abs(norm.z)){
			double a = norm.x / abs(norm.y);
			double b = norm.z / abs(norm.y);
			double d = 2.0 / (cubesize-1);
			for (int i = 0; i < cubesize - 1; ++i){
				double err = 0.02;
				double offset = 0.0;
				if (cubesize % 2 == 1)
					offset = d * (0.1 + abs(1.0 * (cubesize) / 2 - 1 - i)*0.03);
				if (cubesize % 2 == 0)
					offset = d * (0.1 + abs(1.0 * (cubesize-1) / 2 - i)*0.03);
				if (i == 0 || i == cubesize - 2) offset = d * 0.3;
				if (cubesize % 2 == 1)
					err = 0.02 + abs(1.0 * (cubesize) / 2 - 1 - i) * 0.0025;
				if (cubesize % 2 == 0)
					err = 0.02 + abs(1.0 * (cubesize - 1) / 2 - i) * 0.0025;
				if (cubesize % 2 == 1 && i > 1.0 * (cubesize) / 2 - 1) offset = -offset;
				if (cubesize % 2 == 0 && i > 1.0 * (cubesize - 1) / 2) offset = -offset;
				if (cubesize % 2 == 0 && i == (cubesize / 2) - 1){
					err = 0.015;
					offset = 0;
				}
				if (a + 1.0 - (i + 0.5) * d - offset < err && a + 1.0 - (i + 0.5) * d - offset > -err ||
					b + 1.0 - (i + 0.5) * d - offset < err && b + 1.0 - (i + 0.5) * d - offset > -err){
					color = vec3(abs(norm.x), abs(norm.y), abs(norm.z));
					break;
				}
			}
		}
		else if (abs(norm.z) > abs(norm.x) && abs(norm.z) > abs(norm.y)){
			double a = norm.x / abs(norm.z);
			double b = norm.y / abs(norm.z);
			double d = 2.0 / (cubesize-1);
			for (int i = 0; i < cubesize - 1; ++i){
				double err = 0.02;
				double offset = 0.0;
				if (cubesize % 2 == 1)
					offset = d * (0.1 + abs(1.0 * (cubesize) / 2 - 1 - i)*0.03);
				if (cubesize % 2 == 0)
					offset = d * (0.1 + abs(1.0 * (cubesize - 1) / 2 - i)*0.03);
				if (i == 0 || i == cubesize - 2) offset = d * 0.3;
				if (cubesize % 2 == 1)
					err = 0.02 + abs(1.0 * (cubesize) / 2 - 1 - i) * 0.0025;
				if (cubesize % 2 == 0)
					err = 0.02 + abs(1.0 * (cubesize - 1) / 2 - i) * 0.0025;
				if (cubesize % 2 == 1 && i > 1.0 * (cubesize) / 2 - 1) offset = -offset;
				if (cubesize % 2 == 0 && i > 1.0 * (cubesize - 1) / 2) offset = -offset;
				if (cubesize % 2 == 0 && i == (cubesize / 2) - 1){
					err = 0.015;
					offset = 0;
				}
				if (a + 1.0 - (i + 0.5) * d - offset < err && a + 1.0 - (i + 0.5) * d - offset > -err ||
					b + 1.0 - (i + 0.5) * d - offset < err && b + 1.0 - (i + 0.5) * d - offset > -err){
					color = vec3(abs(norm.x), abs(norm.y), abs(norm.z));
					break;
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

	void main(){
		//color = texture(tex, UV).rgb * 10;// *0.000001 + vec3(UV.x, UV.y, 0);

		//CHANGE_Huijie
		//color = vec3(0.8, 0.8, 0.8);
		color = vec3(0.0, 0.0, 0.0);
		float frac = 0.4;
		float colMiddle = 0.9;
		float vMiddle = (cmax - cmin) * frac;
		color = vec3(0.0, 0.0, 0.0);
		float v = texture(tex, UV).x;
		float c = (v - cmin) / (cmax - cmin);
		color = vec3(c, c, c);
		
	}
	);

	//CHANGE_Huijie
	glProg = new ShaderProgram();
	glProg->initFromStrings(slice2DVS, slice2DFS);
	glProg->addAttribute("vertexPosition");
	glProg->addUniform("env");
	glProg->addUniform("cmax");
	glProg->addUniform("cmin");
	glProg->addUniform("cubesize");

	glProg6Faces = new ShaderProgram();
	glProg6Faces->initFromStrings(slice2D6FacesVS, slice2D6FacesFS);
	glProg6Faces->addAttribute("vertexPosition");
	glProg6Faces->addUniform("tex");
	glProg6Faces->addUniform("cmax");
	glProg6Faces->addUniform("cmin");
	
	//CHANGE_Huijie
	/*const char* gridVS =
	GLSL(
	layout(location = 0) in vec3 vertexPosition;
	void main(){
		gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
	}
	);

	const char* gridFS =
	GLSL(
	out vec3 fragColor;
	void main(){
		fragColor = vec3(1.0f, 1.0f, 1.0f);
	}
	);

	glProgGrid = new ShaderProgram();
	glProgGrid->initFromStrings(gridVS, gridFS);
	glProgGrid->addAttribute("vertexPosition");*/
}

void GL2DProjWidget::initializeGL()
{
	initializeOpenGLFunctions();
	loadShaders();
	float3 corners[] = {
		make_float3(-1, -1, -0.1),
		make_float3(1, -1, -0.1),
		make_float3(1, 1, -0.1),
		make_float3(-1, 1, -0.1)
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

//CHANGE_Huijie
void draw_string(const char* str)
{
	for (unsigned i = 0; i<strlen(str); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
};

void GL2DProjWidget::paintGL() {
	//CHANGE_Huijie
	glClear(GL_COLOR_BUFFER_BIT);
	/*glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);*/
	if (0 == type) {
		if (nullptr == tex2d)
			return;
		m_vao->bind();
		glProg6Faces->use();
		glUniform1i(glProg6Faces->uniform("tex"), GLint(0));
		glUniform1f(glProg6Faces->uniform("cmax"), cubeMax);
		glUniform1f(glProg6Faces->uniform("cmin"), 0);
		tex2d->bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		tex2d->unbind();
		glProg6Faces->disable();
		m_vao->release();

		glLineWidth(1.5f);
		//glShadeModel(GL_FLAT);
		//draw grid
		int sizeWidth = 4 * cubeSize;
		int sizeHeight = 3 * cubeSize;
		for (int i = 0; i <= sizeWidth; ++i){
			for (int j = 0; j <= sizeHeight; ++j){
				if ((i >= cubeSize && i <= 2 * cubeSize) && (j <= cubeSize || j >= 2 * cubeSize)){
					if (j != cubeSize || j != sizeHeight || j == 0){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * (j + 1) / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
					if (i != 2 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + ((j%cubeSize) + 1)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + ((j%cubeSize) + 1)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + ((j%cubeSize) + 1)].z);
						glVertex3f((2.0f * (i + 1) / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
				}
				if (((i >= 0 && i <= cubeSize) || (i >= 2*cubeSize && i <= 3*cubeSize)) && (j >= cubeSize && j <= 2 * cubeSize)){
					if (j != 2 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * (j + 1) / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
					if (i != cubeSize || i != 3*cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[(i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].x,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].y,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].z);
						glVertex3f((2.0f * (i + 1) / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
				}
				if (((i >= cubeSize && i <= 2 * cubeSize) || (i >= 3*cubeSize && i <= sizeWidth)) && (j >= cubeSize && j <= 2 * cubeSize)){
					if (j != 2 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * (j + 1) / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
					if (i != 2*cubeSize || i != 4 * sizeWidth){
						glBegin(GL_LINES);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((2.0f * i / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].z);
						glVertex3f((2.0f * (i + 1) / sizeWidth) - 1.0f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
				}
			}
		}



		static char tmp_str1[25];
		static char tmp_str2[25];
		static char tmp_str3[25];
		static char tmp_str4[25];
		static char tmp_str5[25];
		static char tmp_str6[25];
		//glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		//glColor3ub(255, 255, 0);
		glColor3ub(255, 255, 0);

		//renderText(-0.4, 0.0, 0.0, "Z+");

		//mark the faces
		gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
		glRasterPos2f(-0.22, 0.05);
		sprintf(tmp_str1, "+Z");
		draw_string(tmp_str1);

		glRasterPos2f(0.7, 0.43);
		sprintf(tmp_str2, "-Z");
		draw_string(tmp_str2);

		glRasterPos2f(0.2, 0.43);
		sprintf(tmp_str3, "+X");
		draw_string(tmp_str3);

		glRasterPos2f(-0.8, 0.43);
		sprintf(tmp_str4, "-X");
		draw_string(tmp_str4);

		glRasterPos2f(-0.63, -0.68);
		sprintf(tmp_str5, "-Y");
		draw_string(tmp_str5);

		glRasterPos2f(-0.63, 0.66);
		sprintf(tmp_str6, "+Y");
		draw_string(tmp_str6);

		//glEnable(GL_DEPTH_TEST);

	}
	else {
		if (nullptr == tex)
			return;
		m_vao->bind();
		glProg->use();
		glUniform1i(glProg->uniform("env"), GLint(0));
		glUniform1f(glProg->uniform("cmax"), cubeMax);
		glUniform1f(glProg->uniform("cmin"), 0);
		glUniform1i(glProg->uniform("cubesize"), cubeSize);
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

		//mark the faces
		static char tmp_str1[25];
		static char tmp_str2[25];
		static char tmp_str3[25];
		static char tmp_str4[25];
		static char tmp_str5[25];
		static char tmp_str6[25];
		//glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
		//glColor3ub(255, 255, 0);
		glColor3ub(255, 255, 0);

		//renderText(-0.4, 0.0, 0.0, "Z+");

		gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
		glRasterPos2f(0.5, 0.1);
		sprintf(tmp_str1, "+Z");
		draw_string(tmp_str1);

		glRasterPos2f(-0.5, 0.1);
		sprintf(tmp_str2, "-Z");
		draw_string(tmp_str2);

		glRasterPos2f(0.0, 0.1);
		sprintf(tmp_str3, "+X");
		draw_string(tmp_str3);

		glRasterPos2f(-0.95, 0.0);
		sprintf(tmp_str4, "-X");
		draw_string(tmp_str4);
		glRasterPos2f(0.85, 0.0);
		draw_string(tmp_str4);

		glRasterPos2f(0.0, 0.8);
		sprintf(tmp_str5, "-Y");
		draw_string(tmp_str5);

		glRasterPos2f(0.0, -0.9);
		sprintf(tmp_str1, "+Y");
		draw_string(tmp_str1);
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

	QOpenGLWidget::update();
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

	cubeSize = c->cubemap_size;
	//global color map
	//cubeMax = Cube::cubeMaxValue;
	//cubeMin = Cube::cubeMinValue;

	cubeMin = 10;
	cubeMax = -10;
	//local color map
	for (int i = 0; i < cubeSize * cubeSize * 6; ++i){
		if (c->data[i] > cubeMax) cubeMax = c->data[i];
		if (c->data[i] < cubeMin) cubeMin = c->data[i];
	}
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

	//CHANGE_Huijie:compute colors of the cubemap
	cubeColor = new float3[(cubeSize+1) * (cubeSize+1) * 3];
	float step = 2.0 / (cubeSize);
	for (int i = 0; i <= cubeSize; ++i){
		for (int j = 0; j <= cubeSize; ++j){
			float3 norm1 = normalize(make_float3(1.0, 1.0 - j*step, 1.0 - i*step));
			cubeColor[i*(cubeSize+1) + j] = make_float3(abs(norm1.x), abs(norm1.y), abs(norm1.z));

			float3 norm2 = normalize(make_float3(-1.0 + i*step, 1.0, -1.0 + j*step));
			cubeColor[(cubeSize+1)*(cubeSize+1) + i*(cubeSize+1) + j] = make_float3(abs(norm2.x), abs(norm2.y), abs(norm2.z));

			float3 norm3 = normalize(make_float3( 1.0 - i*step, -1.0 + j*step,1.0));
			cubeColor[2*(cubeSize+1)*(cubeSize+1) + i*(cubeSize+1) + j] = make_float3(abs(norm3.x), abs(norm3.y), abs(norm3.z));
		}
	}


	update();
}

int GL2DProjWidget::heightForWidth(int w) const { 
	return w * 0.5; 
	//CHANGE_Huijie
	//return w * 0.75;
}