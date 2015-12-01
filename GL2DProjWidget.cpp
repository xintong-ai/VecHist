#include "GL2DProjWidget.h"
#include <vector_types.h>
#include <vector_functions.h>
#include <GLTextureCube.h>
#include "Cubemap.h"
//CHANGE_Huijie
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
			double d = 2.0 / (cubesize - 1);
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
		else if (abs(norm.y) > abs(norm.x) && abs(norm.y) > abs(norm.z)){
			double a = norm.x / abs(norm.y);
			double b = norm.z / abs(norm.y);
			double d = 2.0 / (cubesize - 1);
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
		else if (abs(norm.z) > abs(norm.x) && abs(norm.z) > abs(norm.y)){
			double a = norm.x / abs(norm.z);
			double b = norm.y / abs(norm.z);
			double d = 2.0 / (cubesize - 1);
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
		//UV = (vertexPosition.xy + 1) * 0.5;
		UV.y = (vertexPosition.y + 1) * 0.5;
		UV.x = (vertexPosition.x + 0.75) / 1.5;
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

	//CHANGE_Huijie
	float3 corners2[] = {
		make_float3(-0.75, -1.0, 0),
		make_float3(0.75, -1.0, 0),
		make_float3(0.75, 1.0, 0),
		make_float3(-0.75, 1.0, 0)
	};

	m_vao2 = new QOpenGLVertexArrayObject();
	m_vao2->create();
	glGenBuffers(1, &vbo_vert2);
	m_vao2->bind();
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vert2);
	glVertexAttribPointer(glProg->attribute("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &corners2[0].x, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(glProg->attribute("vertexPosition"));
	m_vao2->release();

	label1 = new QLabel(this);
	label2 = new QLabel(this);
	label3 = new QLabel(this);
	label4 = new QLabel(this);
	label5 = new QLabel(this);
	label6 = new QLabel(this);
	label7 = new QLabel(this);
	label8 = new QLabel(this);
	label9 = new QLabel(this);
	label10 = new QLabel(this);
	label11 = new QLabel(this);
	label12 = new QLabel(this);
	label13 = new QLabel(this);

	//+Y
	label1->setObjectName(QStringLiteral("label1"));
	label1->setGeometry(QRect(112, 20, 30, 30));
	label1->setText("+Y");
	label1->setAlignment(Qt::AlignCenter);
	QFont ft1("Microsoft YaHei", 10, 63);
	ft1.setPointSize(14);
	label1->setFont(ft1);
	QPalette pa1;
	pa1.setColor(QPalette::WindowText, Qt::yellow);
	label1->setPalette(pa1);

	//-X
	label2->setObjectName(QStringLiteral("label2"));
	label2->setGeometry(QRect(90, 50, 30, 30));
	label2->setText("-X");
	label2->setAlignment(Qt::AlignCenter);
	QFont ft2("Microsoft YaHei", 10, 63);
	ft2.setPointSize(14);
	label2->setFont(ft2);
	QPalette pa2;
	pa2.setColor(QPalette::WindowText, Qt::yellow);
	label2->setPalette(pa2);

	//+Z
	label3->setObjectName(QStringLiteral("label3"));
	label3->setGeometry(QRect(180, 90, 30, 30));
	label3->setText("+Z");
	label3->setAlignment(Qt::AlignCenter);
	QFont ft3("Microsoft YaHei", 10, 63);
	ft3.setPointSize(14);
	label3->setFont(ft3);
	QPalette pa3;
	pa3.setColor(QPalette::WindowText, Qt::yellow);
	label3->setPalette(pa3);

	//+X
	label4->setObjectName(QStringLiteral("label4"));
	label4->setGeometry(QRect(270, 50, 30, 30));
	label4->setText("+X");
	label4->setAlignment(Qt::AlignCenter);
	QFont ft4("Microsoft YaHei", 10, 63);
	ft4.setPointSize(14);
	label4->setFont(ft4);
	QPalette pa4;
	pa4.setColor(QPalette::WindowText, Qt::yellow);
	label4->setPalette(pa4);

	//-Z
	label5->setObjectName(QStringLiteral("label5"));
	label5->setGeometry(QRect(360, 50, 30, 30));
	label5->setText("-Z");
	label5->setAlignment(Qt::AlignCenter);
	QFont ft5("Microsoft YaHei", 10, 63);
	ft5.setPointSize(14);
	label5->setFont(ft5);
	QPalette pa5;
	pa5.setColor(QPalette::WindowText, Qt::yellow);
	label5->setPalette(pa5);

	//-Y
	label6->setObjectName(QStringLiteral("label6"));
	label6->setGeometry(QRect(115, 180, 30, 30));
	label6->setText("-Y");
	label6->setAlignment(Qt::AlignCenter);
	QFont ft6("Microsoft YaHei", 10, 63);
	ft6.setPointSize(14);
	label6->setFont(ft6);
	QPalette pa6;
	pa6.setColor(QPalette::WindowText, Qt::yellow);
	label6->setPalette(pa6);


	label13->setObjectName(QStringLiteral("label13"));
	label13->setGeometry(QRect(220, 205, 30, 30));
	label13->setText("+Y");
	label13->setAlignment(Qt::AlignCenter);
	QFont ft13("Microsoft YaHei", 10, 63);
	ft13.setPointSize(14);
	label13->setFont(ft13);
	QPalette pa13;
	pa13.setColor(QPalette::WindowText, Qt::yellow);
	label13->setPalette(pa13);


	label12->setObjectName(QStringLiteral("label12"));
	label12->setGeometry(QRect(10, 105, 30, 30));
	label12->setText("-X");
	label12->setAlignment(Qt::AlignCenter);
	QFont ft12("Microsoft YaHei", 10, 63);
	ft12.setPointSize(14);
	label12->setFont(ft12);
	QPalette pa12;
	pa12.setColor(QPalette::WindowText, Qt::yellow);
	label12->setPalette(pa12);


	label11->setObjectName(QStringLiteral("label11"));
	label11->setGeometry(QRect(350, 90, 30, 30));
	label11->setText("+Z");
	label11->setAlignment(Qt::AlignCenter);
	QFont ft11("Microsoft YaHei", 10, 63);
	ft11.setPointSize(14);
	label11->setFont(ft11);
	QPalette pa11;
	pa11.setColor(QPalette::WindowText, Qt::yellow);
	label11->setPalette(pa11);

	label10->setObjectName(QStringLiteral("label10"));
	label10->setGeometry(QRect(235, 90, 30, 30));
	label10->setText("+X");
	label10->setAlignment(Qt::AlignCenter);
	QFont ft10("Microsoft YaHei", 10, 63);
	ft10.setPointSize(14);
	label10->setFont(ft10);
	QPalette pa10;
	pa10.setColor(QPalette::WindowText, Qt::yellow);
	label10->setPalette(pa10);

	label9->setObjectName(QStringLiteral("label9"));
	label9->setGeometry(QRect(100, 90, 30, 30));
	label9->setText("-Z");
	label9->setAlignment(Qt::AlignCenter);
	QFont ft9("Microsoft YaHei", 10, 63);
	ft9.setPointSize(14);
	label9->setFont(ft9);
	QPalette pa9;
	pa9.setColor(QPalette::WindowText, Qt::yellow);
	label9->setPalette(pa9);

	label8->setObjectName(QStringLiteral("label8"));
	label8->setGeometry(QRect(220, 12, 30, 30));
	label8->setText("-Y");
	label8->setAlignment(Qt::AlignCenter);
	QFont ft8("Microsoft YaHei", 10, 63);
	ft8.setPointSize(14);
	label8->setFont(ft8);
	QPalette pa8;
	pa8.setColor(QPalette::WindowText, Qt::yellow);
	label8->setPalette(pa8);

	label7->setObjectName(QStringLiteral("label7"));
	label7->setGeometry(QRect(435, 105, 30, 30));
	label7->setText("-X");
	label7->setAlignment(Qt::AlignCenter);
	QFont ft7("Microsoft YaHei", 10, 63);
	ft7.setPointSize(14);
	label7->setFont(ft7);
	QPalette pa7;
	pa7.setColor(QPalette::WindowText, Qt::yellow);
	label7->setPalette(pa7);

	label1->hide();
	label2->hide();
	label3->hide();
	label4->hide();
	label5->hide();
	label6->hide();
	label7->hide();
	label8->hide();
	label9->hide();
	label10->hide();
	label11->hide();
	label12->hide();
	label13->hide();

}

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

		m_vao2->bind();
		glProg6Faces->use();
		glUniform1i(glProg6Faces->uniform("tex"), GLint(0));
		glUniform1f(glProg6Faces->uniform("cmax"), cubeMax);
		glUniform1f(glProg6Faces->uniform("cmin"), 0);
		tex2d->bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		tex2d->unbind();
		glProg6Faces->disable();
		m_vao2->release();

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
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * (j + 1) / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
					if (i != 2 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + ((j%cubeSize) + 1)].x,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + ((j%cubeSize) + 1)].y,
							cubeColor[(cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize))*(cubeSize + 1) + ((j%cubeSize) + 1)].z);
						glVertex3f((1.5f * (i + 1) / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
				}
				if (((i >= 0 && i <= cubeSize) || (i >= 2 * cubeSize && i <= 3 * cubeSize)) && (j >= cubeSize && j <= 2 * cubeSize)){
					if (j != 2 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * (j + 1) / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
					if (i != cubeSize && i != 3 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[(i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].x,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].y,
							cubeColor[(i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].z);
						glVertex3f((1.5f * (i + 1) / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
				}
				if (((i >= cubeSize && i <= 2 * cubeSize) || (i >= 3 * cubeSize && i <= sizeWidth)) && (j >= cubeSize && j <= 2 * cubeSize)){
					if (j != 2 * cubeSize){
						glBegin(GL_LINES);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + ((i%cubeSize) + 1)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * (j + 1) / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
					if (i != 2 * cubeSize && i != sizeWidth){
						glBegin(GL_LINES);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + (j%cubeSize)].z);
						glVertex3f((1.5f * i / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glColor3f(cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].x,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].y,
							cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + (i%cubeSize)*(cubeSize + 1) + ((j%cubeSize) + 1)].z);
						glVertex3f((1.5f * (i + 1) / sizeWidth) - 0.75f, (2.0f * j / sizeHeight) - 1.0f, 0.0f);
						glEnd();
					}
				}
			}
		}
		
		label1->show();
		label2->show();
		label3->show();
		label4->show();
		label5->show();
		label6->show();

		label7->hide();
		label8->hide();
		label9->hide();
		label10->hide();
		label11->hide();
		label12->hide();
		label13->hide();
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

		label7->show();
		label8->show();
		label9->show();
		label10->show();
		label11->show();
		label12->show();
		label13->show();

		label1->hide();
		label2->hide();
		label3->hide();
		label4->hide();
		label5->hide();
		label6->hide();


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
		else{
			texMode = 0;
		}
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
	cubeColor = new float3[(cubeSize + 1) * (cubeSize + 1) * 3];
	float step = 2.0 / (cubeSize);
	for (int i = 0; i <= cubeSize; ++i){
		for (int j = 0; j <= cubeSize; ++j){
			float3 norm1 = normalize(make_float3(1.0, 1.0 - j*step, 1.0 - i*step));
			cubeColor[i*(cubeSize + 1) + j] = make_float3(abs(norm1.x), abs(norm1.y), abs(norm1.z));

			float3 norm2 = normalize(make_float3(-1.0 + i*step, 1.0, -1.0 + j*step));
			cubeColor[(cubeSize + 1)*(cubeSize + 1) + i*(cubeSize + 1) + j] = make_float3(abs(norm2.x), abs(norm2.y), abs(norm2.z));

			float3 norm3 = normalize(make_float3(1.0 - i*step, -1.0 + j*step, 1.0));
			cubeColor[2 * (cubeSize + 1)*(cubeSize + 1) + i*(cubeSize + 1) + j] = make_float3(abs(norm3.x), abs(norm3.y), abs(norm3.z));
		}
	}

	update();
}

int GL2DProjWidget::heightForWidth(int w) const {
	return w * 0.5;
	//CHANGE_Huijie
	//return w * 0.75;
}