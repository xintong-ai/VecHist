#include <qglobal.h>

#include "TextureCubeManager.h"

TextureCubeManager::TextureCubeManager(DataManager * dataManager, int m_maxTextureSize)
{
	this->dataManager = dataManager;
	this->m_maxTextureSize = m_maxTextureSize;
}

//This method loads the leaf nodes list and sets the texture cubes for all data nodes
void TextureCubeManager::UpdateTexture(int application)
{
	blockTex.clear();

	//For flow data, recursively process the tree and store a texture cube for each node
	if (application == 1) {
		UpdateTexture((NodeBi*)dataManager->getRootNode());

		leafNodes = dataManager->GetAllNode();
		cout << "Just fetched all leaf nodes.  The count is: " << leafNodes.size() << endl;
		for (auto nd : leafNodes)	{
			GLTextureCube *texCube = nd->getTexCube();
			glColor3d(1.0, 1.0, 1.0);
			blockTex << texCube;
		}
	}
	//For Dark Sky data, process only the leaves.  We may want to review this again later.
	else {
		leafNodes = dataManager->GetAllNode();
		for (auto nd : leafNodes)	{
			GLTextureCube *texCube = new GLTextureCube(qMin(1024, m_maxTextureSize), 1);
			texCube->load(nd->GetCubemap(), dataManager->GetCubemapSize());
			glColor3d(1.0, 1.0, 1.0);
			blockTex << texCube;
		}
	}
}

//This method is only for flow data.  It recursiely processes the entropy tree and sets the texture cube for each data node.
void TextureCubeManager::UpdateTexture(NodeBi * currentNode)
{
	GLTextureCube *texCube = new GLTextureCube(qMin(1024, m_maxTextureSize), 1);
	if (currentNode->GetCubemap() != nullptr) {
		texCube->load(currentNode->GetCubemap(), dataManager->GetCubemapSize());
	}
	else
	{
		cerr << "In UpdateTexture method, CubeMap not set for Bi Node!!!" << endl;
	}
	currentNode->setTexCube(texCube);

	if (currentNode->left != nullptr) {
		UpdateTexture(currentNode->left);
	}
	if (currentNode->right != nullptr) {
		UpdateTexture(currentNode->right);
	}

}

QVector<GLTextureCube*> TextureCubeManager::getBlockTex()
{
	return blockTex;

}

vector<AbstractNode*> TextureCubeManager::getLeafNodes()
{
	return leafNodes;

}