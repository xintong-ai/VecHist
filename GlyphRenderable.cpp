#include "GlyphRenderable.h"
#include "BoxRenderable.h"
#include "Cubemap.h"
#include "GLTextureCube.h"
#include "glwidget.h"

//removing the following lines will cause runtime error
#ifdef WIN32
#include "windows.h"
#endif

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#define qgl	QOpenGLContext::currentContext()->functions()
#include "ShaderProgram.h"
//#include "MeshReader.h"
#include "GLSphere.h"
#include "helper_math.h"
//#include <GL/glu.h>

///* picking tolerance in pixels: */
//#define PICK_TOL 10.

//#include <GL/glu.h>
//#include <math.h>
//#define EPS 1e-6
//#include <float.h>
//copied from:
//http://stackoverflow.com/questions/19332668/drawing-the-axis-with-its-arrow-using-opengl-in-visual-studio-2010-and-c
//#define RADPERDEG 0.0174533
//void Arrow(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble D)
//{
//	double x = x2 - x1;
//	double y = y2 - y1;
//	double z = z2 - z1;
//	double L = sqrt(x*x + y*y + z*z);
//
//	GLUquadricObj *quadObj;
//
//	glPushMatrix();
//
//	glTranslated(x1, y1, z1);
//
//	if ((x != 0.) || (y != 0.)) {
//		glRotated(atan2(y, x) / RADPERDEG, 0., 0., 1.);
//		glRotated(atan2(sqrt(x*x + y*y), z) / RADPERDEG, 0., 1., 0.);
//	}
//	else if (z<0){
//		glRotated(180, 1., 0., 0.);
//	}
//
//	glTranslatef(0, 0, L - 4 * D);
//
//	quadObj = gluNewQuadric();
//	gluQuadricDrawStyle(quadObj, GLU_FILL);
//	gluQuadricNormals(quadObj, GLU_SMOOTH);
//	gluCylinder(quadObj, 2 * D, 0.0, 4 * D, 32, 1);
//	gluDeleteQuadric(quadObj);
//
//	quadObj = gluNewQuadric();
//	gluQuadricDrawStyle(quadObj, GLU_FILL);
//	gluQuadricNormals(quadObj, GLU_SMOOTH);
//	gluDisk(quadObj, 0.0, 2 * D, 32, 1);
//	gluDeleteQuadric(quadObj);
//
//	glTranslatef(0, 0, -L + 4 * D);
//
//	quadObj = gluNewQuadric();
//	gluQuadricDrawStyle(quadObj, GLU_FILL);
//	gluQuadricNormals(quadObj, GLU_SMOOTH);
//	gluCylinder(quadObj, D, D, L - 4 * D, 32, 1);
//	gluDeleteQuadric(quadObj);
//
//	quadObj = gluNewQuadric();
//	gluQuadricDrawStyle(quadObj, GLU_FILL);
//	gluQuadricNormals(quadObj, GLU_SMOOTH);
//	gluDisk(quadObj, 0.0, D, 32, 1);
//	gluDeleteQuadric(quadObj);
//
//	glPopMatrix();
//
//}

void GlyphRenderable::LoadShaders()
{

#define GLSL(shader) "#version 440\n" #shader
	//shader is from https://www.packtpub.com/books/content/basics-glsl-40-shaders
	//using two sides shading

	//the reason for flat shading is that the normal is not computed right
	//http://stackoverflow.com/questions/4703432/why-does-my-opengl-phong-shader-behave-like-a-flat-shader
	const char* vertexVS =
	GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
	layout(location = 0) in vec3 VertexPosition;
	//layout(location = 1) in vec3 VertexNormal;
	smooth out vec3 tnorm;
	out vec3 norm;
	out vec4 eyeCoords;

	uniform mat4 ModelViewMatrix;
	uniform mat3 NormalMatrix;
	uniform mat4 ProjectionMatrix;
	uniform vec3 Transform;
	uniform float Scale;
	uniform samplerCube env;
	//uniform vec4 cm[33];
	void main()
	{
		vec3 VertexNormal = VertexPosition;
		mat4 MVP = ProjectionMatrix * ModelViewMatrix;
		norm = VertexNormal;
		//vec3 vert_sphere = VertexPosition * (0.2 + v) * 0.5;



		eyeCoords = ModelViewMatrix *
			vec4(VertexPosition, 1.0);
		float v = textureCube(env, norm).x;

		vec3 t;
		if (abs(norm.z) > 0.0001)
			t = normalize(vec3(1, 1,  - (norm.x + norm.y) / norm.z));
		else if (abs(norm.x) > 0.0001)
			t = normalize(vec3(-(norm.y + norm.z) / norm.x, 1, 1));
		else
			t = normalize(vec3(1, -(norm.z + norm.x) / norm.y, 1));
		vec3 b = normalize(cross(t, norm));
		vec3 nt = normalize(VertexPosition + t * 0.05);
		vec3 nb = normalize(VertexPosition + b * 0.05);
		float vt = textureCube(env, nt).x;
		float vb = textureCube(env, nb).x;
		const float ext = 0.1;
		vec3 new_normal = normalize(cross(nb * (ext + vb) - norm * (ext + v), nt* (ext + vt) - norm * (ext + v)));
		tnorm = normalize(NormalMatrix * new_normal);

		//tnorm = normalize(NormalMatrix * VertexNormal);

		gl_Position = MVP * vec4(VertexPosition * (ext + v) * 0.5 * Scale + Transform, 1.0);
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
	smooth in vec3 norm;
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

	vec3 GetColor2(vec3 norm, float v)
	{
		vec3 ret = vec3(0, 0, 0);
//		return vec3(clamp(abs(norm.x) + 1.0f - v, 0, 1), clamp(abs(norm.y) + 1.0f - v, 0, 1), clamp(abs(norm.z) + (1.0f - v), 0, 1));
		//if (v > 0.0001)
		ret = vec3(abs(norm.x), abs(norm.y), abs(norm.z));
		return ret;
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
		vec3 unlitColor = GetColor2(norm, v);
		//vec3 unlitColor = GetColor(v, 0, 1).xyz;// vec3(v, 0, 0); //vec4((normal.x + 1) * 0.5, (normal.y + 1), (normal.z + 1) * 0.5, 1.0f);
		FragColor = vec4(phongModel(unlitColor, eyeCoords, tnorm), 1.0);
	}
	);

	glProg = new ShaderProgram();
	glProg->initFromStrings(vertexVS, vertexFS);

	glProg->addAttribute("VertexPosition");
	//glProg->addAttribute("VertexNormal");

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

	//////////////////Picking shader/////////////////////
	const char* pickingVS =
		GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
	layout(location = 0) in vec3 VertexPosition;
	uniform mat4 ModelViewMatrix;
	uniform mat4 ProjectionMatrix;
	uniform vec3 Transform;
	uniform float Scale;
	uniform samplerCube env;
	void main()
	{
		vec3 VertexNormal = VertexPosition;
		mat4 MVP = ProjectionMatrix * ModelViewMatrix;
		float v = textureCube(env, VertexNormal).x;
		const float ext = 0.1;
		gl_Position = MVP * vec4(VertexPosition * (ext + v) * 0.5 * Scale + Transform, 1.0);
	}
	);

	const char* pickingFS =
		GLSL(
	layout(location = 0) out vec4 FragColor;
	uniform int idx;
	void main() {
		FragColor = vec4((idx % 255) / 255.0f, (idx / 255) / 255.0f, 0.0f, 1.0f);
	}
	);

	glPickingProg = new ShaderProgram();
	glPickingProg->initFromStrings(pickingVS, pickingFS);

	glPickingProg->addAttribute("VertexPosition");
	glPickingProg->addUniform("ModelViewMatrix");
	glPickingProg->addUniform("ProjectionMatrix");

	glPickingProg->addUniform("Transform");
	glPickingProg->addUniform("Scale");
	glPickingProg->addUniform("env");
	glPickingProg->addUniform("idx");


}

void GlyphRenderable::init()
{
	LoadShaders();
	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();

	glyphMesh = new GLSphere(1, cubemap->GetCubemapSize());
	//glyphMesh->SphereMesh(1.0f, 32, 32);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GenVertexBuffer(glyphMesh->GetNumVerts(),
		glyphMesh->GetVerts());

	//glSelectBuffer(PICK_BUFFER_SIZE, PickBuffer);
	//RenderMode = GL_RENDER;
}

GlyphRenderable::GlyphRenderable(Cubemap* r)
{
	SetCubemap(r);
}

void GlyphRenderable::UpdateData()
{
	int dim1 = dataDim[(sliceDimIdx + 1) % 3];
	int dim2 = dataDim[(sliceDimIdx + 2) % 3];
	int n_step = 0;
	if (dim1 < dim2)
		n_step = dim1 / numGlyphPerDim;
	else
		n_step = dim2 / numGlyphPerDim;
	int numGlyphSide1 = dim1 / n_step;
	int numGlyphSide2 = dim2 / n_step;
	//the .clear() does not trigger the destructor, 
	//so we need to delete the pointer first
	for (auto v : cubes) delete v;
	cubes.clear();
	for (int i = 0; i < numGlyphSide1; i++) {
		for (int j = 0; j < numGlyphSide2; j++) {
			int startCoords [] = { 0, 0, 0 };
			startCoords[sliceDimIdx] = sliceStart;
			startCoords[(sliceDimIdx + 1) % 3] = i * n_step;
			startCoords[(sliceDimIdx + 2) % 3] = j * n_step;

			Cube* c = new Cube(startCoords[0], startCoords[1], startCoords[2], n_step, n_step, n_step);
			cubemap->GenCubeMap(c->pos.x, c->pos.y, c->pos.z, c->size.x, c->size.y, c->size.z, c->data, c->cubemap_size);
			cubes.push_back(c);
		}
	}

	for (auto v : textures) delete v;
	textures.clear();
	for (auto v : bboxes) delete v;
	bboxes.clear();
	int cubemap_size = cubemap->GetCubemapSize();
	for (int i = 0; i < cubes.size(); i++) {
		GLTextureCube* tex = new GLTextureCube(cubemap_size);
		tex->load(cubes[i]->data, cubemap_size);
		textures.push_back(tex);
		BoxRenderable* bb = new BoxRenderable(cubes[i]->pos, cubes[i]->size);
		bboxes.push_back(bb);
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

	Renderable::draw(modelview, projection);
	glMatrixMode(GL_MODELVIEW);
	
	//int viewport[4]; /* place to retrieve the viewport numbers */
	//if (RenderMode == GL_SELECT)
	//{
	//	viewport[0] = 0;
	//	viewport[1] = 0;
	//	viewport[2] = winWidth;
	//	viewport[3] = winHeight;
	//	gluPickMatrix((double)Xmouse, (double)(winHeight - 1 - Ymouse),
	//		PICK_TOL, PICK_TOL, viewport);
	//}

	//if (RenderMode == GL_SELECT)
	//{
	//	glInitNames();
	//	glPushName(0xffffffff); /* a strange value */
	//}

	if (cubesVisible) {
		for (auto b : bboxes) {
			b->draw(modelview, projection);
		}
	}
	for (int i = 0; i < cubes.size(); i++) {
		glLoadName(i);
		glPushMatrix();

		GLTextureCube* tex = textures[i];// m_textureCubeManager->getBlockTex()[i];
		Cube* c = cubes[i];
		float3 shift = make_float3(c->size.x * 0.5 + c->pos.x, c->size.y * 0.5 + c->pos.y, c->size.z * 0.5 + c->pos.z);
		float min_dim = std::min(std::min(c->size.x, c->size.y), c->size.z);

		glProg->use();
		m_vao->bind();

		//float3 ka = make_float3(0.2f, 0.2f, 0.2f);
		QMatrix4x4 q_modelview = QMatrix4x4(modelview);
		q_modelview = q_modelview.transposed();
		qgl->glUniform4f(glProg->uniform("LightPosition"), 0, 0, 100, 1);
		//qgl->glUniform3f(glProg->uniform("Ka"), ka.x, ka.y, ka.z);
		qgl->glUniform3f(glProg->uniform("Kd"), 0.4f, 0.4f, 0.4f);
		qgl->glUniform3f(glProg->uniform("Ks"), 0.2f, 0.2f, 0.2f);
		qgl->glUniform1f(glProg->uniform("Shininess"), 1);
		qgl->glUniform3fv(glProg->uniform("Transform"), 1, &shift.x);
		qgl->glUniform1f(glProg->uniform("Scale"), min_dim);
		qgl->glUniform1i(glProg->uniform("env"), GLint(0));
		qgl->glUniformMatrix4fv(glProg->uniform("ModelViewMatrix"), 1, GL_FALSE, modelview);
		qgl->glUniformMatrix4fv(glProg->uniform("ProjectionMatrix"), 1, GL_FALSE, projection);
		qgl->glUniformMatrix3fv(glProg->uniform("NormalMatrix"), 1, GL_FALSE, q_modelview.normalMatrix().data());

		tex->bind();
		glDrawArrays(GL_QUADS, 0, glyphMesh->GetNumVerts());
		//glDrawElements(GL_TRIANGLES, glyphMesh->numElements, GL_UNSIGNED_INT, glyphMesh->indices);
		tex->unbind();
		m_vao->release();
		glProg->disable();
		glPopMatrix();
	}
}


void GlyphRenderable::GenVertexBuffer(int nv, float* vertex)
{
	m_vao->bind();

	qgl->glGenBuffers(1, &vbo_vert);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	qgl->glVertexAttribPointer(glProg->attribute("VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	qgl->glBufferData(GL_ARRAY_BUFFER, nv * sizeof(float) * 3, vertex, GL_STATIC_DRAW);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	qgl->glEnableVertexAttribArray(glProg->attribute("VertexPosition"));

	m_vao->release();
}

void GlyphRenderable::SlotSliceNumChanged(int i)
{
	sliceStart = i;
	UpdateData();
	actor->UpdateGL();
}

void GlyphRenderable::SlotNumPartChanged(int i)
{
	numGlyphPerDim = i;
	UpdateData();
	actor->UpdateGL();
}


void GlyphRenderable::SlotGenCubeAlongLine(float4* line, int nv)
{
	int step = 23;//better to be odd number
	int nGlyph = nv / step - 1;
	for (int i = 0; i < nGlyph; i++) {
		//float4 center = make_float4(0, 0, 0, 1);
		float3 coordMax = make_float3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		float3 coordMin = make_float3(FLT_MAX, FLT_MAX, FLT_MAX);
		for (int j = 0; j < step; j++) {
			float4 v = line[i * step + j];
			//center += v;
			if (v.x > coordMax.x)
				coordMax.x = v.x;
			if (v.y > coordMax.y)
				coordMax.y = v.y;
			if (v.z > coordMax.z)
				coordMax.z = v.z;
			if (v.x < coordMin.x)
				coordMin.x = v.x;
			if (v.y < coordMin.y)
				coordMin.y = v.y;
			if (v.z < coordMin.z)
				coordMin.z = v.z;
		}
		//center /= step;
		float3 blockSize = coordMax - coordMin;
		float blockSizeMax = std::max(std::max(blockSize.x, blockSize.y), blockSize.z);
		blockSize = make_float3(blockSizeMax, blockSizeMax, blockSizeMax);
		float4 tmp = line[int((i + 0.5) * step)];
		//int3 blockCenter = make_int3(int(tmp.x + 0.5), int(tmp.y + 0.5), int(tmp.z + 0.5));// (coordMax + coordMin) * 0.5;
		float3 blockCenter = make_float3(tmp);
		Cube* c = new Cube(
			int(blockCenter.x - blockSize.x * 0.5 + 0.5), 
			int(blockCenter.y - blockSize.y * 0.5 + 0.5),
			int(blockCenter.z - blockSize.z * 0.5 + 0.5),
			int(blockSize.x + 0.5), int(blockSize.y + 0.5), int(blockSize.z + 0.5));
		cubemap->GenCubeMap(c->pos.x, c->pos.y, c->pos.z, c->size.x, c->size.y, c->size.z, c->data, c->cubemap_size);
		cubes.push_back(c);
	}


	UpdateData();
	actor->UpdateGL();
}

void GlyphRenderable::mousePress(int x, int y, int modifier)
{
//	Xmouse = x;
//	Ymouse = y;
//	RenderMode = GL_SELECT;
//	glRenderMode( GL_SELECT );
//	draw(&matrix_mv.v[0].x, &matrix_pj.v[0].x);
//	RenderMode = GL_RENDER;
//	GLint Nhits = glRenderMode( GL_RENDER );
//#ifdef BUG_KLUDGE
//	if( Nhits == 0 )
//	{
//		RenderMode = GL_SELECT;
//		glRenderMode( GL_SELECT );
//		Display();
//		RenderMode = GL_RENDER;
//		Nhits = glRenderMode( GL_RENDER );
//	}
//#endif
//	std::cout << "Nhits: " << Nhits << std::endl;
//	for (int i = 0, index = 0; i < Nhits; i++)
//	{
//		GLuint nitems = PickBuffer[index++];
//		GLuint zmin = PickBuffer[index++];
//		GLuint zmax = PickBuffer[index++];
//		//if( Debug )
//		//{
//		//	fprintf( stderr,
//		//		"Hit # %2d: found %2d items on the name stack\n",
//		//		i, nitems );
//		//	fprintf( stderr, "\tZmin = 0x%0x, Zmax = 0x%0x\n",
//		//		zmin, zmax );
//		//}
//		for (int j = 0; j < nitems; j++)
//		{
//			GLuint  item = PickBuffer[index++];
//			std::cout << "item: " << item << std::endl;
//		}
//	}
//
//				//if( Debug )
//				//	fprintf(stderr, "\t%2d: %6d\n", j, item);
#if 0
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < cubes.size(); i++) {
		glPushMatrix();

		GLTextureCube* tex = textures[i];// m_textureCubeManager->getBlockTex()[i];
		Cube* c = cubes[i];
		float3 shift = make_float3(c->size.x * 0.5 + c->pos.x, c->size.y * 0.5 + c->pos.y, c->size.z * 0.5 + c->pos.z);
		float min_dim = std::min(std::min(c->size.x, c->size.y), c->size.z);

		glPickingProg->use();
		m_vao->bind();

		qgl->glUniform3fv(glPickingProg->uniform("Transform"), 1, &shift.x);
		qgl->glUniform1f(glPickingProg->uniform("Scale"), min_dim);
		qgl->glUniform1i(glPickingProg->uniform("env"), GLint(0));
		qgl->glUniform1i(glPickingProg->uniform("idx"), GLint(i));
		qgl->glUniformMatrix4fv(glPickingProg->uniform("ModelViewMatrix"), 1, GL_FALSE, &matrix_mv.v[0].x);
		qgl->glUniformMatrix4fv(glPickingProg->uniform("ProjectionMatrix"), 1, GL_FALSE, &matrix_pj.v[0].x);

		tex->bind();
		glDrawArrays(GL_QUADS, 0, glyphMesh->GetNumVerts());
		tex->unbind();
		m_vao->release();
		glPickingProg->disable();
		glPopMatrix();
	}
	glFlush();
	glReadBuffer(GL_BACK);
	GLfloat dataRecord[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, dataRecord);
	std::cout << "Click was registered.  X: " << x << " Y: " << y << " Index received: " 
		<< dataRecord[0] << ", " << dataRecord[1] << ", " << dataRecord[2] << ", " << std::endl;
	int idx = dataRecord[0] * 255 + dataRecord[0] * 255 * 255;
	std::cout << "idx:" << idx << std::endl;
	glReadBuffer(GL_FRONT);
#endif
	int idx = textures.size() * 0.5;
	emit SigChangeTex(textures[idx], cubes[idx]);
}