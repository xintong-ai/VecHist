#include <vector_functions.h>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

#define qgl	QOpenGLContext::currentContext()->functions()

#include "ShaderProgram.h"
#include "LineRenderable.h"
#include <QMatrix4x4>
#include "glwidget.h"
//#include "cuda_math.h"
#include "Streamline.h"

#include <cuda_gl_interop.h> //this file cannot be compiled in the .cu file

#define GLSL(shader) "#version 400\n" #shader
//shader is from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

const char* lineVS =
GLSL(
in vec4 vertex;
in vec3 color;
//in vec3 tangent;
//in vec4 vertexObjectColor;

uniform mat4 PROJECTION;
uniform mat4 MODELVIEW;

uniform vec4 line_color;
out vec4 VertexColor;

void main()
{
	//vec3 lightDirection = vec3(0.0f, 1.0f, 0.0f);
	//float diffuseIntensity;
	//float specularItensity;

	//vec3 light;

	//vec3 normal;
	//vec3 eye;

	//vec3 reflection;

	//light = normalize(lightDirection);

	//normal = normalize(tangent);
	//eye = normalize(vec3(0.0f, 1.0f, 0.0f));

	//diffuseIntensity = clamp(max(dot(normal, light), 0.0), 0.0, 1.0);

	//reflection = normalize(reflect(-light, normal));
	//specularItensity = pow(clamp(max(dot(reflection, eye), 0.0), 0.0, 1.0), 2.0);

	//vec4 ks = vec4(0.3, 0.3, 0.3, 1.0);
	//vec4 kd = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	//vec4 ka = vec4(0.5, 0.5, 0.5, 1.0);
	//vec4 cc;
	//if (line_color.a > 0.6)
	//	cc = vertexObjectColor;
	//else
	//	cc = vec4(line_color.x, line_color.y, line_color.z, 1.0);

	//VetexColor = ka * cc
	//	+ kd*diffuseIntensity
	//	+ ks*(specularItensity);
	if (line_color.y < 0.001)
		VertexColor = line_color;
	else
		VertexColor = vec4(color, 1.0f);
	//VertexColor = vec4(0,1,1,1);
	gl_Position = PROJECTION * MODELVIEW * vertex;
	//gl_Position = gl_Position / gl_Position.w;
}


);

//the texture only returns vec4, so we should change the texture input format
const char* lineFS =
GLSL(
in vec4 VertexColor;
out vec4 FragColor;


void main()
{
	FragColor = VertexColor;
}

);

template <typename T>
T determinantOfMinor(int          theRowHeightY,
	int          theColumnWidthX,
	const T theMatrix[/*Y=*/3][/*X=*/3])
{
	int x1 = theColumnWidthX == 0 ? 1 : 0;  /* always either 0 or 1 */
	int x2 = theColumnWidthX == 2 ? 1 : 2;  /* always either 1 or 2 */
	int y1 = theRowHeightY == 0 ? 1 : 0;  /* always either 0 or 1 */
	int y2 = theRowHeightY == 2 ? 1 : 2;  /* always either 1 or 2 */

	return (theMatrix[y1][x1] * theMatrix[y2][x2])
		- (theMatrix[y1][x2] * theMatrix[y2][x1]);
}

template <typename T>
T determinant(const T theMatrix[/*Y=*/3][/*X=*/3])
{
	return (theMatrix[0][0] * determinantOfMinor(0, 0, theMatrix))
		- (theMatrix[0][1] * determinantOfMinor(0, 1, theMatrix))
		+ (theMatrix[0][2] * determinantOfMinor(0, 2, theMatrix));
}

template <typename T>
bool inverse(const T theMatrix[/*Y=*/3][/*X=*/3],
	T theOutput[/*Y=*/3][/*X=*/3])
{
	T det = determinant(theMatrix);

	/* Arbitrary for now.  This should be something nicer... */
	if (abs(det) < 1e-2)
	{
		memset(theOutput, 0, sizeof theOutput);
		return false;
	}

	T oneOverDeterminant = 1.0 / det;

	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
		{
		/* Rule is inverse = 1/det * minor of the TRANSPOSE matrix.  *
		* Note (y,x) becomes (x,y) INTENTIONALLY here!              */
		theOutput[y][x]
			= determinantOfMinor(x, y, theMatrix) * oneOverDeterminant;

		/* (y0,x1)  (y1,x0)  (y1,x2)  and (y2,x1)  all need to be negated. */
		if (1 == ((x + y) % 2))
			theOutput[y][x] = -theOutput[y][x];
		}

	return true;
}

void LineRenderable::UpdateData()
{
//	int numVerts = dataReader->GetNumVerts();

//	float4* vc = dataReader->GetVertCoords();
//	int dataSize[3];
	//volumeReader->GetDataSize(dataSize);
	//float3* data = volumeReader->GetColors();

	//float3 spacedir[3];
	//volumeReader->GetDataSpaceDir(spacedir);
	//float3 dataOrigin;
	//volumeReader->GetDataOrigin(dataOrigin);

	//float spaceDirArr[3][3] =
	//{
	//	{ spacedir[0].x, spacedir[0].y, spacedir[0].z },
	//	{ spacedir[1].x, spacedir[1].y, spacedir[1].z },
	//	{ spacedir[2].x, spacedir[2].y, spacedir[2].z },
	//};
	//float spaceDirInvArr[3][3];

	//inverse(spaceDirArr, spaceDirInvArr);

	//matrix3x3 spaceDirInvMat;
	//spaceDirInvMat.v[0] = make_float3(spaceDirInvArr[0][0], spaceDirInvArr[0][1], spaceDirInvArr[0][2]);
	//spaceDirInvMat.v[1] = make_float3(spaceDirInvArr[1][0], spaceDirInvArr[1][1], spaceDirInvArr[1][2]);
	//spaceDirInvMat.v[2] = make_float3(spaceDirInvArr[2][0], spaceDirInvArr[2][1], spaceDirInvArr[2][2]);

	std::vector<float4> verts;
	int cnt = 0;
	for (auto l : lines) {
		verts.insert(verts.begin(), l.begin(), l.end());
		lineOffsets.push_back(cnt);
		cnt += l.size();
	}
	int numVerts = verts.size();
	float3* colorArr = new float3[numVerts];
	//int sz01 = dataSize[0] * dataSize[1];
	//TODO:this is using nearest neighbor, we should use linear interpolation later
	for (int i = 0; i < numVerts; i++) {
		//float3 vc2 = spaceDirInvMat * (make_float3(vc[i].x, vc[i].y, vc[i].z) - dataOrigin);
		colorArr[i] = make_float3(1,1,1);// data[int(vc2.z) * sz01 + int(vc2.y) * dataSize[0] + int(vc2.x)];
	}

	m_vao->bind();

	qgl->glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
	qgl->glVertexAttribPointer(glProg->attribute("vertex"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	qgl->glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * numVerts, 
		(float*)&verts[0], GL_STATIC_DRAW);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	qgl->glEnableVertexAttribArray(glProg->attribute("vertex")); 

	qgl->glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	qgl->glVertexAttribPointer(glProg->attribute("color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	qgl->glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * numVerts,
		(float*)colorArr, GL_STATIC_DRAW);
	qgl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	qgl->glEnableVertexAttribArray(glProg->attribute("color"));

	m_vao->release();

	delete [] colorArr;
}


void LineRenderable::init()
{
	m_vao = new QOpenGLVertexArrayObject();
	m_vao->create();

	qgl->glGenBuffers(1, &vbo_vert);
	qgl->glGenBuffers(1, &vbo_color);

	loadShaders();

	//int3 dim = ->GetVolumeDim();
	float3 seeds[1] = { make_float3(63, 63, 256) };// dim.x * 0.5, dim.x * 0.5, dim.x * 0.5)};
	std::vector<std::vector<float4>> newlines = lineGenerator->GenStreamWithSeeds(seeds, 1);
	lines.insert(lines.begin(), newlines.begin(), newlines.end());
	UpdateData();

	//struct cudaGraphicsResource *cuda_vbo_obj_resource;
	//struct cudaGraphicsResource *cuda_vbo_color_resource;
	//cudaGraphicsGLRegisterBuffer(&cuda_vbo_obj_resource,
	//	vbo_vert, cudaGraphicsMapFlagsWriteDiscard);
	//cudaGraphicsGLRegisterBuffer(&cuda_vbo_color_resource,
	//	vbo_color, cudaGraphicsMapFlagsWriteDiscard);

	//_primitiveLengths.assign(dataReader->GetNumVertPerLine(), dataReader->GetNumVertPerLine() + dataReader->GetNumLines());
	//_primitiveOffsets.assign(dataReader->GetLineBaseIdx(), dataReader->GetLineBaseIdx() + dataReader->GetNumLines());

	//_deformLine->SetGLResource(cuda_vbo_obj_resource, cuda_vbo_color_resource, dataReader->GetNumVerts(),
	//	dataReader->GetLineBaseIdx(), dataReader->GetNumVertPerLine(), dataReader->GetNumLines());
}

void LineRenderable::loadShaders()
{
	glProg = new ShaderProgram();
	glProg->initFromStrings(lineVS, lineFS);

	glProg->addAttribute("vertex");
	glProg->addAttribute("color");
	glProg->addUniform("line_color");
	glProg->addUniform("MODELVIEW");
	glProg->addUniform("PROJECTION");
	
}

void LineRenderable::resize(int width, int height)
{
	//_deformLine->SetWinSize(width, height);
}

void LineRenderable::draw(float modelview[16], float projection[16])
{
	if (!visible)
		return;
	Renderable::draw(modelview, projection);
	//_deformLine->SetMatrix(modelview, projection);
	//_deformLine->RunCuda(matrix_mv, matrix_pj, transSize);

	glProg->use();
	m_vao->bind();

	qgl->glUniformMatrix4fv(glProg->uniform("MODELVIEW"), 1, GL_FALSE, modelview);
	qgl->glUniformMatrix4fv(glProg->uniform("PROJECTION"), 1, GL_FALSE, projection);

	//Draw halo
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LINE_BIT); //push the Depth buffer test function
    glDepthFunc(GL_LEQUAL);
	glLineWidth(5);
	qgl->glUniform4f(glProg->uniform("line_color"), 0, 0, 0, 1);
	for (int il = 0; il < lines.size(); il++) {
		glDrawArrays(GL_LINE_STRIP, lineOffsets[il], lines[il].size());
	}

	//Draw lines
	glLineWidth(3);
	qgl->glUniform4f(glProg->uniform("line_color"), 0, 1, 1, 1);
	for (int il = 0; il < lines.size(); il++) {
		glDrawArrays(GL_LINE_STRIP, lineOffsets[il], lines[il].size());
	}
	glPopAttrib();

	m_vao->release();
	glProg->disable();
}

void LineRenderable::cleanup()
{
}

LineRenderable::LineRenderable(Streamline* r)
{
	SetLineGenerator(r);
}


void LineRenderable::mousePress(int x, int y, int modifier)
{
}

void LineRenderable::mouseRelease(int x, int y, int modifier)
{
}

void LineRenderable::mouseMove(int x, int y, int modifier)
{
}

void LineRenderable::GenGlyphAlongLine(int i)
{
	emit SigGenCubeAlongLine(&lines[i][0], lines[i].size());
}

