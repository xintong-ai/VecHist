#include "Streamline.h"
#include <OSUFlow.h>
#include <vector_types.h>
#include <vector_functions.h>

using namespace std;
void Streamline::LoadOSUFlow(const char* filename)
{
	osuflow = new OSUFlow();
	list<vtListSeedTrace*> sl_list;

	// load the scalar field
	//	LOG(printf("read file %s\n", argv[1]));

	osuflow->LoadData(filename, true); //true: a steady flow field 
	osuflow->NormalizeField(true);

	////szVecFilePath = argv[1];	// ADD-BY-LEETEN 09/29/2012

	// comptue the bounding box of the streamlines 
	VECTOR3 minB, maxB;
	VECTOR3 minLen, maxLen;
	float center[3], len[3];
	osuflow->Boundary(minLen, maxLen); // get the boundary 
	minB[0] = minLen[0]; minB[1] = minLen[1];  minB[2] = minLen[2];
	maxB[0] = maxLen[0]; maxB[1] = maxLen[1];  maxB[2] = maxLen[2];
	//  osuflow->SetBoundary(minB, maxB);  // set the boundary. just to test
	// the subsetting feature of OSUFlow
#ifdef DEBUG_PRINTING
	printf(" volume boundary X: [%f %f] Y: [%f %f] Z: [%f %f]\n",
		minLen[0], maxLen[0], minLen[1], maxLen[1],
		minLen[2], maxLen[2]);
#endif

	center[0] = (minLen[0] + maxLen[0]) / 2.0;
	center[1] = (minLen[1] + maxLen[1]) / 2.0;
	center[2] = (minLen[2] + maxLen[2]) / 2.0;
#ifdef DEBUG_PRINTING
	printf("center is at %f %f %f \n", center[0], center[1], center[2]);
#endif
	len[0] = maxLen[0] - minLen[0];
	len[1] = maxLen[1] - minLen[1];
	len[2] = maxLen[2] - minLen[2];


//	float from[3], to[3];
//
//	from[0] = minLen[0];   from[1] = minLen[1];   from[2] = minLen[2];
//	to[0] = maxLen[0];   to[1] = maxLen[1];   to[2] = maxLen[2];
//
//	printf("generating seeds...\n");
//	size_t seedDim[3] = { 5, 5, 5 };
//	osuflow->SetRegularSeedPoints(from, to, seedDim);
//	int nSeeds;
//	VECTOR3* seeds = osuflow->GetSeeds(nSeeds);
//	for (int i = 0; i<nSeeds; i++)
//#ifdef DEBUG_PRINTING
//		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0],
//		seeds[i][1], seeds[i][2]);
//#endif
//
//	sl_list.clear();
//
//	printf("compute streamlines..\n");
//	osuflow->SetIntegrationParams(1, 5);
//	//osuflow->GenStreamLines(sl_list, BACKWARD_AND_FORWARD, 200, 0);
//	osuflow->GenStreamLines(sl_list, BACKWARD_AND_FORWARD, 100, 0);
//	printf(" done integrations\n");
//	printf("list size = %d\n", (int)sl_list.size());
//
//	int iT = 0;
//	for (list<vtListSeedTrace*>::const_iterator
//		pIter = sl_list.begin();
//		pIter != sl_list.end();
//	pIter++, iT++)
//	{
//		const vtListSeedTrace *trace = *pIter;
//		int iP = 0;
//		vector<float4> line;
//		for (list<VECTOR3*>::const_iterator
//			pnIter = trace->begin();
//			pnIter != trace->end();
//		pnIter++, iP++)
//		{
//			VECTOR3 p = **pnIter;
//			line.push_back(make_float4(p[0], p[1], p[2], 1.0f));
//		}
//		if (line.size() > 16)
//			streamlines.push_back(line);
//	}
}

Streamline::Streamline(const char* filename)
{
	LoadOSUFlow(filename);

}

std::vector < std::vector<float4> > Streamline::GenStreamWithSeeds(float3* seeds, int num)
{
	std::vector < std::vector<float4> > ret;
	list<vtListSeedTrace*> sl_list;

	osuflow->SetSeedPoints((VECTOR3*)seeds, num);
	printf("compute streamlines..\n");
	osuflow->SetIntegrationParams(1, 1);
	//osuflow->GenStreamLines(sl_list, BACKWARD_AND_FORWARD, 50, 0);
	osuflow->GenStreamLines(sl_list, FORWARD_DIR, 1000, 0);
	printf(" done integrations\n");

	int iT = 0;
	for (list<vtListSeedTrace*>::const_iterator
		pIter = sl_list.begin();
		pIter != sl_list.end();
	pIter++, iT++)
	{
		const vtListSeedTrace *trace = *pIter;
		int iP = 0;
		vector<float4> line;
		for (list<VECTOR3*>::const_iterator
			pnIter = trace->begin();
			pnIter != trace->end();
		pnIter++, iP++)
		{
			VECTOR3 p = **pnIter;
			line.push_back(make_float4(p[0], p[1], p[2], 1.0f));
		}
		ret.push_back(line);
	}
	return ret;
}


//void Streamline::GenStreamInCube(int qCubePos[3], int qCubeSize[3])
//{
//	printf("generating seeds...\n");
//	list<vtListSeedTrace*> sl_list;
//	float from[3], to[3];
//	//int qCubePos[3], qCubeSize[3];	//queried cube position and sizes
//
//	from[0] = qCubePos[0];   from[1] = qCubePos[1];   from[2] = qCubePos[2];
//	to[0] = qCubePos[0] + qCubeSize[0];   to[1] = qCubePos[1] + qCubeSize[1];   to[2] = qCubePos[2] + qCubeSize[2];
//
//	osuflow->SetRandomSeedPoints(from, to, 16);
//	int nSeeds;
//	VECTOR3* seeds = osuflow->GetSeeds(nSeeds);
//#ifdef DEBUG_PRINTING
//	for (int i = 0; i<nSeeds; i++)
//		printf(" seed no. %d : [%f %f %f]\n", i, seeds[i][0],
//		seeds[i][1], seeds[i][2]);
//#endif
//
//	sl_list.clear();
//
//	printf("compute streamlines..\n");
//	osuflow->SetIntegrationParams(1, 5);
//	//osuflow->GenStreamLines(sl_list, BACKWARD_AND_FORWARD, 50, 0);
//	osuflow->GenStreamLines(sl_list, FORWARD_DIR, 100, 0);
//	printf(" done integrations\n");
//	printf("list size = %d\n", (int)sl_list.size());
//
//	int iT = 0;
//	streamlinesInCube.clear();
//	for (list<vtListSeedTrace*>::const_iterator
//		pIter = sl_list.begin();
//		pIter != sl_list.end();
//	pIter++, iT++)
//	{
//		const vtListSeedTrace *trace = *pIter;
//		int iP = 0;
//		vector<float4> line;
//		for (list<VECTOR3*>::const_iterator
//			pnIter = trace->begin();
//			pnIter != trace->end();
//		pnIter++, iP++)
//		{
//			VECTOR3 p = **pnIter;
//			line.push_back(make_float4(p[0], p[1], p[2], 1.0f));
//		}
//		streamlinesInCube.push_back(line);
//	}
//}

//vector<vector<float4>> Streamline::GetStreamlines()
//{
//	return streamlines;
//}

//vector<vector<float4>> Streamline::GetStreamlinesInCube()
//{
//	return streamlinesInCube;
//}
