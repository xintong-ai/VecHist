
#include "glsphere.h"
#include "vector_types.h"
#include "vector_functions.h"
#include "helper_math.h"// to define normalize()


//float lerp(float a, float b, float t)
//{
//	return a * (1.0f - t) + b * t;
//}


GLSphere::GLSphere(float r, int n)
//: GLTriangleMesh<P3T2N3Vertex, unsigned short>((n + 1) * (n + 1) * 6, 36 * n * n)
{
	//build the regular mesh in the constructor
	//using namespace std;
	float x, y, z;
	float n_rev = 1.0f / n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float3 v0 = normalize(make_float3(0.5, i * n_rev - 0.5, j * n_rev - 0.5));
			float3 v1 = normalize(make_float3(0.5, (i + 1) * n_rev - 0.5, j * n_rev - 0.5));
			float3 v2 = normalize(make_float3(0.5, i * n_rev - 0.5, (j + 1) * n_rev - 0.5));
			float3 v3 = normalize(make_float3(0.5, (i + 1) * n_rev - 0.5, (j + 1) * n_rev - 0.5));
			grid.push_back(v0);
			grid.push_back(v1);
			grid.push_back(v3);
			grid.push_back(v2);
		}
	}
	//mirror by the plane
	for (int i = 0; i < 4 * n * n; i++) {
		grid.push_back(make_float3(-grid[i].x, grid[i].y, grid[i].z));
	}
	//rotate x, y, z
	for (int i = 0; i < 8 * n * n; i++) {
		grid.push_back(make_float3(grid[i].z, grid[i].x, grid[i].y));
	}
	for (int i = 0; i < 8 * n * n; i++) {
		grid.push_back(make_float3(grid[i].y, grid[i].z, grid[i].x));
	}
	//for (int i = 0; i <= nSub; i++)	{
	//	double iAng = (0.5 + (float)i / nSub) * CUDART_PIO2;
	//	vector<float3> row;
	//	for (int j = 0; j <= nSub; j++)	{
	//		float jAng = (-0.5 + (float)j / nSub) * CUDART_PIO2;
	//		float v1 = pow(tan(iAng), 2);
	//		float v2 = pow(tan(jAng), 2);
	//		float x = sqrt(1 / (1 + v1 + v1 * v2));
	//		if (iAng > CUDART_PIO2)
	//			x = -x;
	//		y = x * tan(iAng);
	//		z = y * tan(jAng);
	//		//			row.push_back(make_float3(x, y, z));
	//		verticesReg.push_back(make_float3(x, y, z));
	//		int first = (nSub + 1) * j + i;
	//		if (i != nSub && j != nSub)	{
	//			faces.push_back(make_int3(first, first + nSub + 1, first + 1));// , first + nSub + 1));
	//			faces.push_back(make_int3(first + 1, first + nSub + 1, first + nSub + 2));
	//		}
	//	}
	//	//		grid.push_back(row);
	//}

	//scale = 0.2;
	//int vidx = 0, iidx = 0;

	//P3T2N3Vertex *vp = m_vb.lock();
	//unsigned short *ip = m_ib.lock();

	//if (!vp || !ip) {
	//	qWarning("GLRoundedBox::GLRoundedBox: Failed to lock vertex buffer and/or index buffer.");
	//	m_ib.unlock();
	//	m_vb.unlock();
	//	return;
	//}

	//float x, y, z;
	//for (int f = 0; f < 6; f++)
	//{
	//	for (int i = 0; i <= n; i++)	{
	//		float x = (float)i / n - 0.5;
	//		for (int j = 0; j <= n; j++)	{
	//			float y = (float)j / n - 0.5;
	//			int fcoord = f / 2;
	//			int fsign = pow(-1, f % 2);
	//			QVector3D v(1.0,1.0,1.0);
	//			//v.setX(0.5 * pow(-1, fsign));
	//			//v[0] = 0.5;// *pow(-1, fsign);
	//			if (0 == fcoord)	{
	//				v.setX(0.5 * fsign);
	//				v.setY(x * fsign);
	//				v.setZ(y);
	//			}
	//			else if (1 == fcoord)	{
	//				v.setX(y);
	//				v.setY(0.5 * fsign);
	//				v.setZ(x * fsign);
	//			}
	//			else{
	//				v.setX(x * fsign);
	//				v.setY(y);
	//				v.setZ(0.5 * fsign);
	//			}
	//			//grid.push_back(v);
	//			vp[vidx].normal = v.normalized();
	//			vp[vidx].position = vp[vidx].normal * scale;
	//			vp[vidx].texCoord = vp[vidx].normal;
	//			vidx++;
	//			//int first = (nSub + 1) * j + i;
	//			if (i != n && j != n)	{
	//				ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j;
	//				ip[iidx++] = f * (n + 1) * (n + 1) + (i + 1) * (n + 1) + j;
	//				ip[iidx++] = f * (n + 1) * (n + 1) + (i + 1) * (n + 1) + j + 1;

	//				ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j;
	//				ip[iidx++] = f * (n + 1) * (n + 1) + (i + 1) * (n + 1) + j + 1;
	//				ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j + 1;

	//				//ip[iidx++] = f * (n + 1) * (n + 1) + (i - 1) * (n + 1) + j;
	//				//ip[iidx++] = f * (n + 1) * (n + 1) + i * (n + 1) + j + 1;
	//				//ip[iidx++] = f * (n + 1) * (n + 1) + (i - 1) * (n + 1) + j + 1;
	//				//faces.push_back(make_int3(first, first + nSub + 1, first + 1));// , first + nSub + 1));
	//				//faces.push_back(make_int3(first + 1, first + nSub + 1, first + nSub + 2));
	//			}
	//		}
	//		//		grid.push_back(row);
	//	}
	//}

	//m_ib.unlock();
	//m_vb.unlock();
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

