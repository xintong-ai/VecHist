#ifndef HIST_MESH_H
#define HIST_MESH_H

#include <vector>
#include <vector_types.h>
//using namespace std;

//template <Typename float>
inline std::vector<std::vector<float>> Float3ToVector(std::vector<float3> v)
{
	std::vector<std::vector<float>> ret;
	for (int i = 0; i < v.size(); i++)	{
		ret.push_back(std::vector<float>{ v[i].x, v[i].y, v[i].z });
	}
	return ret;
}

inline std::vector<std::vector<int>> Int4ToVector(std::vector<int4> v)
{
	std::vector<std::vector<int>> ret;
	for (int i = 0; i < v.size(); i++)	{
		ret.push_back(std::vector<int>{ v[i].x, v[i].y, v[i].z, v[i].w});
	}
	return ret;
}

inline std::vector<std::vector<int>> Int3ToVector(std::vector<int3> v)
{
	std::vector<std::vector<int>> ret;
	for (int i = 0; i < v.size(); i++)	{
		ret.push_back(std::vector<int>{ v[i].x, v[i].y, v[i].z});
	}
	return ret;
}

//this class take a vector field set as input, and output the distribution as a mesh
class HistMesh
{
	std::vector<float3> vertices;
	std::vector<int3> faces;

	std::vector<float3> verticesReg;
	int nSub;
public:
	//HistMesh();
	HistMesh(int level);
	~HistMesh();

	void ComputeHistMesh(float* data, int size, 
		std::vector<float3> &vertices, std::vector<std::vector<int>> &faces);

	void ComputeHistMesh(std::vector<float3> data,
		std::vector<float3> &vertices2, std::vector<std::vector<int>> &faces);

	void VisualizeMesh();
};

#endif