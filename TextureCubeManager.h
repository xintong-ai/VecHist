#ifndef TEXTURE_CUBE_MAN
#define TEXTURE_CUBE_MAN

#include <qvector.h>
#include <vector>

#include "glbuffers.h"
#include "DataManager.h"
#include "DataMgrVect.h"

using namespace std;

class TextureCubeManager
{
private:
	QVector<GLTextureCube*> blockTex;
	vector<AbstractNode*> leafNodes;
	DataManager * dataManager;
	int m_maxTextureSize = 0;
public:
	TextureCubeManager(DataManager * dataManager, int m_maxTextureSize);
	void UpdateTexture(int application);
	void UpdateTexture(NodeBi * currentNode);
	QVector<GLTextureCube*> getBlockTex();
	vector<AbstractNode*> getLeafNodes();
};


#endif