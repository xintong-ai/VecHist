
#include "GlyphRenderable.h"
#include "Cubemap.h"
#include <QOpenGLWidget>
//#include "glbuffers.h"
//#include "glSuperquadric.h"
#include "helper_math.h"
//#include "glsphere.h"
#include "GLTextureCube.h"
#ifdef WIN32
#include "windows.h"
#endif

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#define qgl	QOpenGLContext::currentContext()->functions()
#include "ShaderProgram.h"
#include "MeshReader.h"
void GlyphRenderable::LoadShaders()
{

#define GLSL(shader) "#version 440\n" #shader
	//shader is from https://www.packtpub.com/books/content/basics-glsl-40-shaders
	//using two sides shading

	//the reason for flat shading is that the normal is not computed right
	//http://stackoverflow.com/questions/4703432/why-does-my-opengl-phong-shader-behave-like-a-flat-shader
	const char* vertexVS =
		GLSL(
		layout(location = 0) in vec3 VertexPosition;
	layout(location = 1) in vec3 VertexNormal;
	smooth out vec3 tnorm;
	out vec3 norm;
	out vec4 eyeCoords;

	uniform mat4 ModelViewMatrix;
	uniform mat3 NormalMatrix;
	uniform mat4 ProjectionMatrix;
	uniform vec3 Transform;
	void main()
	{
		mat4 MVP = ProjectionMatrix * ModelViewMatrix;
		norm = VertexNormal;
		tnorm = normalize(NormalMatrix * normalize(VertexNormal));

		eyeCoords = ModelViewMatrix *
			vec4(VertexPosition, 1.0);

		gl_Position = MVP * vec4(VertexPosition + Transform, 1.0);
	}
	);

	const char* vertexFS =
		GLSL(
		//#extension GL_ARB_shading_language_include : enable \n
		#extension GL_NV_shadow_samplers_cube : enable \n
		//#extension GL_NV_shadow_samplers_cube : enable
		uniform vec4 LightPosition; // Light position in eye coords.
	//uniform vec3 Ka; // Diffuse reflectivity
	uniform vec3 Kd; // Diffuse reflectivity
	uniform vec3 Ks; // Diffuse reflectivity
	uniform float Shininess;
	in vec4 eyeCoords;
	in vec3 norm;

	smooth in vec3 tnorm;

	layout(location = 0) out vec4 FragColor;

	uniform samplerCube env;

	vec3 phongModel(vec3 a, vec4 position, vec3 normal) {
		vec3 s = normalize(vec3(LightPosition - position));
		vec3 v = normalize(-position.xyz);
		vec3 r = reflect(-s, normal);
		vec3 ambient = a;// Ka;
		float sDotN = max(dot(s, normal), 0.0);
		vec3 diffuse = Kd * sDotN;
		vec3 spec = vec3(0.0);
		if (sDotN > 0.0)
			spec = Ks *
			pow(max(dot(r, v), 0.0), Shininess);
		return ambient + diffuse + spec;
	}

	void main() {
		vec3 FrontColor;
		vec3 BackColor;
		//tnorm = normalize(tnorm);
		//vec3 tmp = vec3(1,0,0);
		float v = textureCube(env, norm).x;
		vec3 unlitColor = vec3(v, 0, 0); //vec4((normal.x + 1) * 0.5, (normal.y + 1), (normal.z + 1) * 0.5, 1.0f);

		FrontColor = phongModel(unlitColor, eyeCoords, tnorm);

		BackColor = phongModel(unlitColor, eyeCoords, -tnorm);

		if (gl_FrontFacing) {
			FragColor = vec4(FrontColor, 1.0);//vec4(tnorm, 1.0);
		}
		else {
			FragColor = vec4(BackColor, 1.0);
		}
	}
	);

	glProg = new ShaderProgram();
	glProg->initFromStrings(vertexVS, vertexFS);

	glProg->addAttribute("VertexPosition");
	glProg->addAttribute("VertexNormal");

	glProg->addUniform("LightPosition");
	//glProg->addUniform("Ka");
	glProg->addUniform("Kd");
	glProg->addUniform("Ks");
	glProg->addUniform("Shininess");

	glProg->addUniform("ModelViewMatrix");
	glProg->addUniform("NormalMatrix");
	glProg->addUniform("ProjectionMatrix");

	glProg->addUniform("Transform");
	glProg->addUniform("env");
}

void GlyphRenderable::init()
{
	LoadShaders();
	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();

	//glEnable(GL_DEPTH_TEST);
	m = new MeshReader();
	m->SphereMesh(1.0f, 16, 16);

	GenVertexBuffer(m->numVerts,
		m->Faces_Triangles,
		m->Normals);
	//delete m;
}

GlyphRenderable::GlyphRenderable(Cubemap* r)
{
	SetCubemap(r);
}

void GlyphRenderable::UpdateData()
{
	textures.clear();
	int cubemap_size = cubemap->GetCubemapSize();
	std::vector<Cube*> cubes = cubemap->GetCubes();
	for (int i = 0; i < cubes.size(); i++) {
		GLTextureCube* tex = new GLTextureCube(cubemap_size);
		tex->load(cubes[i]->data, cubemap_size);
		textures.push_back(tex);
	}
	//GLSuperquadric* mesh = new GLSuperquadric()
}


void GlyphRenderable::draw(float modelview[16], float projection[16])
{
	if (!updated) {
		UpdateData();

	}
	if (!visible)
		return;
	//GLSphere glyph(1.0f, 0.5f, 10);
	std::vector<Cube*> cubes = cubemap->GetCubes();
	for (int i = 0; i < cubes.size(); i++) {
		GLTextureCube* tex = textures[i];// m_textureCubeManager->getBlockTex()[i];
		//auto nd = m_textureCubeManager->getLeafNodes()[i];
		Cube* c = cubes[i];
		//glPushMatrix();
		float3 shift = make_float3(c->size.x * 0.5 + c->pos.x, c->size.y * 0.5 + c->pos.y, c->size.z * 0.5 + c->pos.z);
		//glTranslatef(shift.x, shift.y, shift.z);
		//float min_dim = min(min(c->size.x, c->size.y), c->size.z);
		//glScalef(min_dim, min_dim, min_dim);


		glMatrixMode(GL_MODELVIEW);

		glProg->use();
		m_vao->bind();

		float3 ka = make_float3(0.2f, 0.2f, 0.2f);
		qgl->glUniform4f(glProg->uniform("LightPosition"), 0, 0, 10, 1);
		//qgl->glUniform3f(glProg->uniform("Ka"), ka.x, ka.y, ka.z);
		qgl->glUniform3f(glProg->uniform("Kd"), 0.6f, 0.6f, 0.6f);
		qgl->glUniform3f(glProg->uniform("Ks"), 0.2f, 0.2f, 0.2f);
		qgl->glUniform1f(glProg->uniform("Shininess"), 1);

		qgl->glUniform3fv(glProg->uniform("Transform"), 1, &shift.x);

		qgl->glUniform1i(glProg->uniform("env"), GLint(0));
		QMatrix4x4 q_modelview = QMatrix4x4(modelview);
		q_modelview = q_modelview.transposed();

		qgl->glUniformMatrix4fv(glProg->uniform("ModelViewMatrix"), 1, GL_FALSE, modelview);
		qgl->glUniformMatrix4fv(glProg->uniform("ProjectionMatrix"), 1, GL_FALSE, projection);
		qgl->glUniformMatrix3fv(glProg->uniform("NormalMatrix"), 1, GL_FALSE, q_modelview.normalMatrix().data());
		tex->bind();
		//glDrawArrays(GL_TRIANGLES, 0, m->TotalConnectedTriangles * 3);
		glDrawElements(GL_TRIANGLES, m->numElements, GL_UNSIGNED_INT, m->indices);
		tex->unbind();
		//glBindVertexArray(0);
		m_vao->release();
		glProg->disable();


		//glPopMatrix();
	}
}


void GlyphRenderable::GenVertexBuffer(int nv, float* vertex, float* normal)
{
	m_vao->bind();

	qgl->glGenBuffers(1, &vbo_vert);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	qgl->glVertexAttribPointer(glProg->attribute("VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	qgl->glBufferData(GL_ARRAY_BUFFER, nv * sizeof(float) * 3, vertex, GL_STATIC_DRAW);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	qgl->glEnableVertexAttribArray(glProg->attribute("VertexPosition"));

	qgl->glGenBuffers(1, &vbo_norm);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
	qgl->glVertexAttribPointer(glProg->attribute("VertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	qgl->glBufferData(GL_ARRAY_BUFFER, nv * sizeof(float) * 3, normal, GL_STATIC_DRAW);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	qgl->glEnableVertexAttribArray(glProg->attribute("VertexNormal"));

	m_vao->release();
}