#include "GlyphRenderable.h"
#include "BoxRenderable.h"
#include "Cubemap.h"
#include "GLTextureCube.h"
#include "glwidget.h"

//removing the following lines will cause runtime error
#ifdef WIN32
#include "windows.h"
#endif
#define qgl	QOpenGLContext::currentContext()->functions()

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include "ShaderProgram.h"
#include "GLSphere.h"
#include "helper_math.h"

void GlyphRenderable::LoadShaders()
{

#define GLSL(shader) "#version 440\n" #shader
	//shader is from https://www.packtpub.com/books/content/basics-glsl-40-shaders
	//using two sides shading
	const char* vertexVS =
	GLSL(
	layout(location = 0) in vec3 VertexPosition;


	void main()
	{
		gl_Position = vec4(VertexPosition, 1.0f);// ProjectionMatrix * ModelViewMatrix * vec4(posDeformed * Scale + Transform, 1.0);
	}
	);

	//https://www.opengl.org/wiki/Geometry_Shader
	const char* vertexGS = 
		GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
	layout(lines_adjacency) in;
	layout(triangle_strip, max_vertices = 4) out;
	flat out vec3 f_norm;
	flat out vec3 tnorm;
	out vec4 eyeCoords;
	uniform mat4 ModelViewMatrix;
	uniform mat4 ProjectionMatrix;
	uniform mat3 NormalMatrix;
	uniform vec3 Transform;
	uniform float Scale;
	uniform samplerCube env;
	smooth out float height;
	uniform int mapOrder;
	uniform int heightScale;

	vec3 GetDeformed(vec3 dir, float v){
		//we use sqrt(), because the projected area is proportional to the square of the radius.
		//return dir * (0.04 + sqrt(v) * heightScale * 0.1);
		return dir * (0.05 + v * 1.8);
	}

	vec4 GetNDCPos(vec3 v, float h) {
		return ProjectionMatrix * ModelViewMatrix * vec4(GetDeformed(v, h) * Scale + Transform, 1.0);
	}

	void main(void)
	{
		vec3 A = gl_in[0].gl_Position.xyz;
		vec3 B = gl_in[1].gl_Position.xyz;
		vec3 C = gl_in[2].gl_Position.xyz;
		vec3 D = gl_in[3].gl_Position.xyz;
		f_norm = normalize(cross(C - A, D - B));
		if (dot(f_norm, A) < 0)	//in case the normal points to inside the sphere
			f_norm = -f_norm;
		tnorm = normalize(NormalMatrix * f_norm);
		eyeCoords = ModelViewMatrix * vec4(f_norm, 1.0);
		height = pow(textureCube(env, gl_in[0].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;// 0.333);
		gl_Position = GetNDCPos(gl_in[0].gl_Position.xyz, height);
		EmitVertex();
		height = pow(textureCube(env, gl_in[1].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;//0.333);
		gl_Position = GetNDCPos(gl_in[1].gl_Position.xyz, height);
		EmitVertex();
		height = pow(textureCube(env, gl_in[3].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;//0.333);
		gl_Position = GetNDCPos(gl_in[3].gl_Position.xyz, height);
		EmitVertex();
		height = pow(textureCube(env, gl_in[2].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;//0.333);
		gl_Position = GetNDCPos(gl_in[2].gl_Position.xyz, height);
		EmitVertex();
		EndPrimitive();
	}
	);

	const char* vertexFS =
		GLSL(
	#extension GL_NV_shadow_samplers_cube : enable \n
	uniform vec4 LightPosition; // Light position in eye coords.
	uniform vec3 Ka; // Diffuse reflectivity
	uniform vec3 Kd; // Diffuse reflectivity
	uniform vec3 Ks; // Diffuse reflectivity
	uniform float Shininess;
	uniform samplerCube env;
	uniform float aniRatio;
	uniform int mapOrder;
	uniform bool animationOn;

	in vec4 eyeCoords;
	in vec3 f_norm;
	in vec3 tnorm;
	smooth in float height;
	layout(location = 0) out vec4 FragColor;


	vec3 GetColor2(vec3 norm) {
		return vec3(abs(norm.x), abs(norm.y), abs(norm.z));
	}

	vec3 phongModel(vec3 a, vec4 position, vec3 normal) {
		vec3 s = normalize(vec3(LightPosition - position));
		vec3 v = normalize(-position.xyz);
		vec3 r = reflect(-s, normal);
		vec3 ambient = a * Ka;
		float sDotN = max(dot(s, normal), 0.0);
		vec3 diffuse = Kd * sDotN;
		vec3 spec = vec3(0.0);
		if (sDotN > 0.0)
			spec = Ks *
			pow(max(dot(r, v), 0.0), Shininess);
		return ambient + diffuse + spec;
	}

	void main() {
		vec3 unlitColor = GetColor2(f_norm);
		FragColor = vec4(phongModel(unlitColor, eyeCoords, tnorm) /** 0.5*/, 1.0);
		//FragColor = vec4(phongModel(unlitColor, eyeCoords, tnorm), 1.0);
		if (animationOn) {
			float maxHeight = pow(1.0f, 1.0f / mapOrder);
			float width = 0.1 * maxHeight;
			float lowerHeight = aniRatio * maxHeight;
			if ((height > lowerHeight) && (height < (lowerHeight + width)))
			{
				float r = (height - lowerHeight) / width;
				FragColor = (1 - r) * FragColor + r * vec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		//lowerHeight = (aniRatio + 0.5) * maxHeight;
		//if ((height > lowerHeight) && (height < (lowerHeight + width)))
		//{
		//	float r = (height - lowerHeight) / width;
		//	FragColor = (1 - r) * FragColor + r * vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//}
	}
	);

	glProg = new ShaderProgram();
	glProg->initFromStrings(vertexVS, vertexFS, vertexGS);

	glProg->addAttribute("VertexPosition");
	glProg->addUniform("LightPosition");
	glProg->addUniform("Ka");
	glProg->addUniform("Kd");
	glProg->addUniform("Ks");
	glProg->addUniform("Shininess");

	glProg->addUniform("ModelViewMatrix");
	glProg->addUniform("NormalMatrix");
	glProg->addUniform("ProjectionMatrix");

	glProg->addUniform("Transform");
	glProg->addUniform("Scale");
	glProg->addUniform("env");
	glProg->addUniform("heightScale");
	glProg->addUniform("aniRatio");
	glProg->addUniform("mapOrder");
	glProg->addUniform("animationOn");
	

	//CHANGE_Huijie: shaders for picking color
	const char* pickingVS =
		GLSL(
		layout(location = 0) in vec3 VertexPosition;

	void main()
	{
		gl_Position = vec4(VertexPosition, 1.0f);
	}
	);

	const char* pickingGS =
		GLSL(
		#extension GL_NV_shadow_samplers_cube : enable \n
		layout(lines_adjacency) in;
	layout(triangle_strip, max_vertices = 4) out;
	uniform mat4 ModelViewMatrix;
	uniform mat4 ProjectionMatrix;
	uniform vec3 Transform;
	uniform float Scale;
	uniform samplerCube env;
	uniform int heightScale;
	uniform int mapOrder;

	vec3 GetDeformed(vec3 dir, float v){
		return dir * (0.05 + v * 1.8);
	}

	vec4 GetNDCPos(vec3 v, float h) {
		return ProjectionMatrix * ModelViewMatrix * vec4(GetDeformed(v, h) * Scale + Transform, 1.0);
	}

	void main(void)
	{
		vec3 A = gl_in[0].gl_Position.xyz;
		vec3 B = gl_in[1].gl_Position.xyz;
		vec3 C = gl_in[2].gl_Position.xyz;
		vec3 D = gl_in[3].gl_Position.xyz;
		float height;
		height = pow(textureCube(env, gl_in[0].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;
		gl_Position = GetNDCPos(gl_in[0].gl_Position.xyz, height);
		EmitVertex();
		height = pow(textureCube(env, gl_in[1].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;
		gl_Position = GetNDCPos(gl_in[1].gl_Position.xyz, height);
		EmitVertex();
		height = pow(textureCube(env, gl_in[3].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;
		gl_Position = GetNDCPos(gl_in[3].gl_Position.xyz, height);
		EmitVertex();
		height = pow(textureCube(env, gl_in[2].gl_Position.xyz).x, 1.0f / mapOrder) * heightScale * 0.1;
		gl_Position = GetNDCPos(gl_in[2].gl_Position.xyz, height);
		EmitVertex();
		EndPrimitive();
	}
	);

	const char* pickingFS =
		GLSL(
		layout(location = 0) out vec4 FragColor;
		uniform vec4 PickingColor;

		void main() {
			FragColor = PickingColor;
		}
	);

	glPickingProg = new ShaderProgram();
	glPickingProg->initFromStrings(pickingVS, pickingFS, pickingGS);

	glPickingProg->addAttribute("VertexPosition");

	glPickingProg->addUniform("ModelViewMatrix");
	glPickingProg->addUniform("ProjectionMatrix");

	glPickingProg->addUniform("Transform");
	glPickingProg->addUniform("Scale");
	glPickingProg->addUniform("env");
	glPickingProg->addUniform("PickingColor");
	glPickingProg->addUniform("mapOrder");
	glPickingProg->addUniform("heightScale");
}

void GlyphRenderable::init()
{
	LoadShaders();
	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();

	glyphMesh = new GLSphere(1, cubemap->GetCubemapSize());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GenVertexBuffer(glyphMesh->GetNumVerts(),
		glyphMesh->GetVerts());
}

GlyphRenderable::GlyphRenderable(Cubemap* r)
{
	SetCubemap(r);
}

void GlyphRenderable::UpdateData()
{
	int dim0 = dataDim[sliceDimIdx];
	int dim1 = dataDim[(sliceDimIdx + 1) % 3];
	int dim2 = dataDim[(sliceDimIdx + 2) % 3];
	int n_step = 0;
	//use the smallest dimension
	if (dim1 < dim2 )//&& dim1 < dim0)
		n_step = dim1 / numGlyphPerDim;
	else// if (dim2 < dim0 && dim2 < dim1)
		n_step = dim2 / numGlyphPerDim;
	if (n_step > dim0)
		return;
		//else
	//	n_step = dim0 / numGlyphPerDim;
	std::cout << "n_step: " << n_step << std::endl;
	int numGlyphSide1 = dim1 / n_step;
	int numGlyphSide2 = dim2 / n_step;
	//the .clear() does not trigger the destructor, 
	//so we need to delete the pointer first
	if (0 == cubemap->GetMode()) {
		for (auto v : cubes) delete v;
		cubes.clear();
		for (int i = 0; i < numGlyphSide1; i++) {
			for (int j = 0; j < numGlyphSide2; j++) {
				int startCoords[] = { 0, 0, 0 };
				startCoords[sliceDimIdx] = sliceStart;
				startCoords[(sliceDimIdx + 1) % 3] = i * n_step;
				startCoords[(sliceDimIdx + 2) % 3] = j * n_step;

				Cube* c = new Cube(startCoords[0], startCoords[1], startCoords[2], n_step, n_step, n_step);
				cubemap->GenCubeMapOptimized(c->pos.x, c->pos.y, c->pos.z, c->size.x, c->size.y, c->size.z, c->data, c->cubemap_size, c->mag);
				cubes.push_back(c);
			}
		}
	}
	else {
		cubes.clear();
		float3 block_start, block_size;
		(&(block_start.x))[sliceDimIdx] = sliceStart;
		(&(block_start.x))[(sliceDimIdx + 1) % 3] = 0;
		(&(block_start.x))[(sliceDimIdx + 2) % 3] = 0;
		(&(block_size.x))[sliceDimIdx] = numGlyphPerDim;
		(&(block_size.x))[(sliceDimIdx + 1) % 3] = dim1;
		(&(block_size.x))[(sliceDimIdx + 2) % 3] = dim2;
		if (2 == sliceStart)
			sliceStart = 4;
		cubemap->GetCubes(block_start.x, block_start.y, block_start.z, block_size.x, block_size.y, block_size.z, cubes);
		//cubemap->GetCubes(0,0,0,63,63,63, cubes);

	}
	for (auto c : cubes)
		c->phase = rand() % aniTimerScale;

	for (auto v : textures) delete v;
	textures.clear();
	for (auto v : bboxes) delete v;
	bboxes.clear();
	int cubemap_size = cubemap->GetCubemapSize();
	for (int i = 0; i < cubes.size(); i++) {
		if (!cubes[i]->data)
			continue;
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

		QMatrix4x4 q_modelview = QMatrix4x4(modelview);
		q_modelview = q_modelview.transposed();
		qgl->glUniform4f(glProg->uniform("LightPosition"), 0, 0, std::max(std::max(dataDim[0], dataDim[1]), dataDim[2]) * 2, 1);
		qgl->glUniform3f(glProg->uniform("Ka"), 0.8f, 0.8f, 0.8f);
		qgl->glUniform3f(glProg->uniform("Kd"), 0.1f, 0.1f, 0.1f);
		qgl->glUniform3f(glProg->uniform("Ks"), 0.2f, 0.2f, 0.2f);
		qgl->glUniform1f(glProg->uniform("Shininess"), 5);
		qgl->glUniform3fv(glProg->uniform("Transform"), 1, &shift.x);
		qgl->glUniform1f(glProg->uniform("Scale"), min_dim);
		qgl->glUniform1i(glProg->uniform("heightScale"), heightScale);
		qgl->glUniform1f(glProg->uniform("aniRatio"), (float)((aniTimer + c->phase) % (aniTimerScale)) / (aniTimerScale - 1));
		
		//qgl->glUniform1i(glProg->uniform("heightScale"), heightScale);
		qgl->glUniform1i(glProg->uniform("env"), GLint(0));
		qgl->glUniform1i(glProg->uniform("mapOrder"), mapOrder);
		qgl->glUniform1i(glProg->uniform("animationOn"), animationOn);
		qgl->glUniformMatrix4fv(glProg->uniform("ModelViewMatrix"), 1, GL_FALSE, modelview);
		qgl->glUniformMatrix4fv(glProg->uniform("ProjectionMatrix"), 1, GL_FALSE, projection);
		qgl->glUniformMatrix3fv(glProg->uniform("NormalMatrix"), 1, GL_FALSE, q_modelview.normalMatrix().data());

		tex->bind();
		glDrawArrays(GL_LINES_ADJACENCY, 0, glyphMesh->GetNumVerts());
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
	std::cout << "sliceStart: " << sliceStart<<std::endl;
	Cube::cubeMaxValue = -10;
	Cube::cubeMinValue = 10;
	UpdateData();
	actor->UpdateGL();
}

void GlyphRenderable::SlotNumPartChanged(int i)
{
	numGlyphPerDim = i;
	//CHANGE_Huijie
	Cube::cubeMaxValue = -10;
	Cube::cubeMinValue = 10;
	UpdateData();
	actor->UpdateGL();
}

void GlyphRenderable::SlotHeightScaleChanged(int i)
{
	heightScale = i;
	actor->UpdateGL();
}

void GlyphRenderable::SlotSetSliceOrie(int i)
{
	if (i != sliceDimIdx && i >= 0 && i < 3) {
		sliceDimIdx = i;
		//sliceStart = 0;
		//CHANGE_Huijie
		Cube::cubeMaxValue = -10;
		Cube::cubeMinValue = 10;
		UpdateData();
		actor->UpdateGL();
	}
}

void GlyphRenderable::SlotSetCubesVisible(bool visible)
{
	if (cubesVisible != visible) {
		cubesVisible = visible;
		actor->UpdateGL();
	}
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

//CHANGE_Huijie
void GlyphRenderable::drawPicking(float modelview[16], float projection[16])
{
	std::cout << "draw picking" << std::endl;

	qgl->glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	qgl->glVertexAttribPointer(glPickingProg->attribute("VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	qgl->glEnableVertexAttribArray(glPickingProg->attribute("VertexPosition"));


	for (int i = 0; i < cubes.size(); i++) {
		glLoadName(i);
		glPushMatrix();

		GLTextureCube* tex = textures[i];// m_textureCubeManager->getBlockTex()[i];
		Cube* c = cubes[i];
		float3 shift = make_float3(c->size.x * 0.5 + c->pos.x, c->size.y * 0.5 + c->pos.y, c->size.z * 0.5 + c->pos.z);
		float min_dim = std::min(std::min(c->size.x, c->size.y), c->size.z);

		glPickingProg->use();
		m_vao->bind();

		int r = ((i + 1) & 0x000000FF) >> 0;
		int g = ((i + 1) & 0x0000FF00) >> 8;
		int b = ((i + 1) & 0x00FF0000) >> 16;

		QMatrix4x4 q_modelview = QMatrix4x4(modelview);
		q_modelview = q_modelview.transposed();
		qgl->glUniform3fv(glPickingProg->uniform("Transform"), 1, &shift.x);
		qgl->glUniform1f(glPickingProg->uniform("Scale"), min_dim);
		qgl->glUniform1i(glPickingProg->uniform("env"), GLint(0));
		qgl->glUniform1i(glPickingProg->uniform("mapOrder"), mapOrder);
		qgl->glUniformMatrix4fv(glPickingProg->uniform("ModelViewMatrix"), 1, GL_FALSE, modelview);
		qgl->glUniformMatrix4fv(glPickingProg->uniform("ProjectionMatrix"), 1, GL_FALSE, projection);
		qgl->glUniform4f(glPickingProg->uniform("PickingColor"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		qgl->glUniform1i(glPickingProg->uniform("heightScale"), heightScale);

		tex->bind();
		glDrawArrays(GL_LINES_ADJACENCY, 0, glyphMesh->GetNumVerts());
		//glDrawElements(GL_TRIANGLES, glyphMesh->numElements, GL_UNSIGNED_INT, glyphMesh->indices);
		tex->unbind();
		m_vao->release();
		glPickingProg->disable();
		glPopMatrix();
	}

	//qgl->glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	//qgl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GlyphRenderable::mousePress(int x, int y, int modifier)
{
	//TODO: add picking functions here to replace the line below

	//int idx = textures.size() * 0.5;
	//emit SigChangeTex(textures[idx], cubes[idx]);

	//CHANGE_Huijie
	if (pickID == 0)
		return;
	else
		emit SigChangeTex(textures[pickID - 1], cubes[pickID - 1]);
}

void GlyphRenderable::animate()
{
	aniTimer = (aniTimer + 1) % aniTimerScale;
}

void GlyphRenderable::SetAnimationOn(bool b) { 
	animationOn = b; 
	actor->UpdateGL();
}
