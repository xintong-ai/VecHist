
#include "GlyphRenderable.h"
#include "Cubemap.h"
#include "GLTextureCube.h"

//removing the following lines will cause runtime error
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
	uniform float Scale;
	//uniform vec4 cm[33];
	void main()
	{
		mat4 MVP = ProjectionMatrix * ModelViewMatrix;
		norm = VertexNormal;
		tnorm = normalize(NormalMatrix * normalize(VertexNormal));

		eyeCoords = ModelViewMatrix *
			vec4(VertexPosition, 1.0);

		gl_Position = MVP * vec4(VertexPosition * Scale + Transform, 1.0);
	}
	);

	const char* vertexFS =
		GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
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

	vec4 GetColor(float v, float vmin, float vmax)
	{
		vec4 c = vec4(0.0, 0.0, 0.0, 1.0);
		float dv;
		dv = vmax - vmin;
		vec4 cm[3];
		cm[0] = vec4(0, 0, 1, 1);
		cm[1] = vec4(1, 1, 1, 1);
		cm[2] = vec4(1, 0, 0, 1);

		float pdv = 0.5 * dv;

		int i = 0;
		for (i = 0; i < 2; i++) {
			if (v >= (vmin + i*pdv) && v < (vmin + (i + 1)*pdv)) {
				c.r = cm[i].r + (v - vmin - i*pdv) / pdv * (cm[i + 1].r - cm[i].r);
				c.g = cm[i].g + (v - vmin - i*pdv) / pdv * (cm[i + 1].g - cm[i].g);
				c.b = cm[i].b + (v - vmin - i*pdv) / pdv * (cm[i + 1].b - cm[i].b);

				break;
			}
		}
		if (v == vmax) {
			c.r = cm[2].r;
			c.g = cm[2].g;
			c.b = cm[2].b;
		}
		return(c);
	}

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
		float v = textureCube(env, norm).x;
		vec3 unlitColor = GetColor(v, 0, 1).xyz;// vec3(v, 0, 0); //vec4((normal.x + 1) * 0.5, (normal.y + 1), (normal.z + 1) * 0.5, 1.0f);
		FragColor = vec4(phongModel(unlitColor, eyeCoords, tnorm), 1.0);
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
	glProg->addUniform("Scale");
	glProg->addUniform("env");
}

void GlyphRenderable::init()
{
	LoadShaders();
	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();

	//glEnable(GL_DEPTH_TEST);
	glyphMesh = new MeshReader();
	glyphMesh->SphereMesh(1.0f, 32, 32);

	GenVertexBuffer(glyphMesh->numVerts,
		glyphMesh->Faces_Triangles,
		glyphMesh->Normals);
	//delete m;
}

GlyphRenderable::GlyphRenderable(Cubemap* r)
{
	SetCubemap(r);
	//int3 pos = make_int3(55, 55, 300);
	//int3 size = make_int3(10, 10, 10);
	Cube* c = new Cube(55, 55, 300, 10, 10, 10);
	cubemap->GenCubeMap(c->pos.x, c->pos.y, c->pos.z, c->size.x, c->size.y, c->size.z, c->data, c->cubemap_size);
	cubes.push_back(c);
	//Cube *c = new Cube(, cubemap->GetCubemapSize());
}

void GlyphRenderable::UpdateData()
{
	textures.clear();
	int cubemap_size = cubemap->GetCubemapSize();
	//std::vector<Cube*> cubes = cubemap->GetCubes();
	for (int i = 0; i < cubes.size(); i++) {
		GLTextureCube* tex = new GLTextureCube(cubemap_size);
		tex->load(cubes[i]->data, cubemap_size);
		textures.push_back(tex);
	}
}


void GlyphRenderable::draw(float modelview[16], float projection[16])
{
	if (!updated) {
		UpdateData();
		updated = true;
	}
	if (!visible)
		return;
	//std::vector<Cube*> cubes = cubemap->GetCubes();
	glMatrixMode(GL_MODELVIEW);
	for (int i = 0; i < cubes.size(); i++) {
		glPushMatrix();

		GLTextureCube* tex = textures[i];// m_textureCubeManager->getBlockTex()[i];
		Cube* c = cubes[i];
		float3 shift = make_float3(c->size.x * 0.5 + c->pos.x, c->size.y * 0.5 + c->pos.y, c->size.z * 0.5 + c->pos.z);
		//glTranslatef(shift.x, shift.y, shift.z);
		float min_dim = std::min(std::min(c->size.x, c->size.y), c->size.z);
		//glScalef(min_dim, min_dim, min_dim);

		glProg->use();
		m_vao->bind();

		float3 ka = make_float3(0.2f, 0.2f, 0.2f);
		QMatrix4x4 q_modelview = QMatrix4x4(modelview);
		q_modelview = q_modelview.transposed();
		qgl->glUniform4f(glProg->uniform("LightPosition"), 0, 0, 10, 1);
		//qgl->glUniform3f(glProg->uniform("Ka"), ka.x, ka.y, ka.z);
		qgl->glUniform3f(glProg->uniform("Kd"), 0.6f, 0.6f, 0.6f);
		qgl->glUniform3f(glProg->uniform("Ks"), 0.2f, 0.2f, 0.2f);
		qgl->glUniform1f(glProg->uniform("Shininess"), 1);
		qgl->glUniform3fv(glProg->uniform("Transform"), 1, &shift.x);
		qgl->glUniform1f(glProg->uniform("Scale"), min_dim);
		qgl->glUniform1i(glProg->uniform("env"), GLint(0));
		qgl->glUniformMatrix4fv(glProg->uniform("ModelViewMatrix"), 1, GL_FALSE, modelview);
		qgl->glUniformMatrix4fv(glProg->uniform("ProjectionMatrix"), 1, GL_FALSE, projection);
		qgl->glUniformMatrix3fv(glProg->uniform("NormalMatrix"), 1, GL_FALSE, q_modelview.normalMatrix().data());

		tex->bind();
		//glDrawArrays(GL_TRIANGLES, 0, m->TotalConnectedTriangles * 3);
		glDrawElements(GL_TRIANGLES, glyphMesh->numElements, GL_UNSIGNED_INT, glyphMesh->indices);
		tex->unbind();
		m_vao->release();
		glProg->disable();


		glPopMatrix();
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