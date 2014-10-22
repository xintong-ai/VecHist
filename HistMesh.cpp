#include "HistMesh.h"
#include "math_constants.h"
#include "vector_functions.h"
#include <igl/viewer/Viewer.h>
#include <igl/list_to_matrix.h>

void HistMesh::ComputeHistMesh(std::vector<float3> data,
	std::vector<float3> &vertices, std::vector<std::vector<int>> &faces)
{

}

HistMesh::HistMesh(int level)
{
	//build the regular mesh in the constructor
	using namespace std;
	int nSub = pow(2, level);
	vector<vector<float3>> grid;
	float x, y, z;
	for (int i = 0; i <= nSub; i++)	{
		double iAng = (0.5 + (float)i / nSub) * CUDART_PIO2;
		vector<float3> row;
		for (int j = 0; j <= nSub; j++)	{
			float jAng = (-0.5 + (float) j / nSub) * CUDART_PIO2;
			float v1 = pow(tan(iAng), 2);
			float v2 = pow(tan(jAng), 2);
			float x = sqrt(1 / (1 + v1 + v1 * v2));
			if (iAng > CUDART_PIO2)
				x = -x;
			y = x * tan(iAng);
			z = y * tan(jAng);
//			row.push_back(make_float3(x, y, z));
			verticesReg.push_back(make_float3(x, y, z));
			int first = (nSub + 1) * j + i;
			if (i != nSub && j != nSub)	{
				faces.push_back(make_int3(first, first + nSub + 1, first + 1));// , first + nSub + 1));
				faces.push_back(make_int3(first + 1, first + nSub + 1, first + nSub + 2));
			}
		}
//		grid.push_back(row);
	}
	VisualizeMesh();
	//	verticesReg.push_back
}



void HistMesh::VisualizeMesh()
{
		Eigen::MatrixXd V;
		Eigen::MatrixXi F;
	//	igl::readOFF("D:/lib/libigl/tutorial/shared/bunny.off", V, F);
		igl::list_to_matrix(Float3ToVector(verticesReg), V);
		igl::list_to_matrix(Int3ToVector(faces), F);
		// Plot the mesh
		igl::Viewer viewer;
		viewer.data.set_mesh(V, F);
		viewer.launch();
	

}