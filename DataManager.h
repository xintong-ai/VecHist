#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <vector>

class DataManager
{
	float* data;
	std::vector<std::vector<double> > vV;
	std::vector<std::vector<int> > vF;
	std::vector<float> vertexValue;
	std::vector<std::vector<double> > vVScaled;
public:
	void LoadVec(char* filename);
	std::vector<std::vector<double> > GetVertexPos();
	std::vector<std::vector<double> > GetScaledVertexPos();
	std::vector<std::vector<int> > GetFaceVertices();
	std::vector<float> GetVertexValue();
	~DataManager();
};

#endif //DATA_MANAGER_H