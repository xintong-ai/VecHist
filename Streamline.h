#ifndef STREAMLINE_H
#define STREAMLINE_H
class OSUFlow;
#include <vector>
#include <vector_types.h>

class Streamline
{
	OSUFlow *osuflow;
	std::vector < std::vector<float4> > streamlines;
	std::vector < std::vector<float4> > streamlinesInCube;

	void LoadOSUFlow(const char* filename);
public:
	Streamline(const char* filename);
	~Streamline();
	void GenStreamInCube(int qCubePos[3], int qCubeSize[3]);
	std::vector<std::vector<float4>> GetStreamlines();
	std::vector<std::vector<float4>> GetStreamlinesInCube();

};

#endif //STREAMLINE_H