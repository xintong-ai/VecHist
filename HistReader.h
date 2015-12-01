#ifndef VEC_READER_H
#define VEC_READER_H
#include <fstream>
#include <Reader.h>
#include <vector_types.h>
#include <vector_functions.h>
class VecReader:public Reader// VecReader:public DataManager
{
	float* data;
	int start[3];
	int dim[3];
	std::string vecFileName;
public:
	VecReader(const char* filename);// AppSettings * appSettings);
	~VecReader();

	void LoadVec(const char* filename);
	void LoadData();
	int3 GetVolumeDim()	{return make_int3(dim[0], dim[1], dim[2]);}
	bool CubeInsideVolume(int x, int y, int z, int nx, int ny, int nz);

	int GetNumOfCells();

	std::string GetFileName(){ return vecFileName; }
	float* GetVecData(){return data;}
};

#endif //VEC_READER_H