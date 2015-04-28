#include "DataMgrCosm.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

DataMgrCosm::DataMgrCosm()
{
}

DataMgrCosm::~DataMgrCosm()
{
}

void DataMgrCosm::LoadData()
{
	cubemap_size = 16;
	//TODO: remove the hard coded part
	//dim[0] = 62.5;
	//dim[1] = 62.5;
	//dim[2] = 62.5;

	LoadHalos();
	LoadMergeTree();

	//haloTable.clear();
}

void DataMgrCosm::LoadHalos()
{
	
	using namespace std;
	ifstream fin(GetStringVal("halo").c_str());
	assert(!fin.fail());
	float data;
	//fin >> data;        // get first number from the file (priming the input statement)
	string line;
	std::getline(fin, line);
	stringstream ss(line);

	float tmp;

	ss >> tmp;
	start[0] = tmp;
	
	ss >> tmp;
	dim[0] = tmp;
	
	ss >> tmp;
	start[1] = tmp;
	
	ss >> tmp;
	dim[1] = tmp;
	
	ss >> tmp;
	start[2] = tmp;
	
	ss >> tmp;
	dim[2] = tmp;

	std::getline(fin, line);

	// You must attempt to read info prior to an eof( ) test.
	while (!fin.eof())      //if not at end of file, continue reading numbers
	{
		stringstream ss(line);
//		cout << line << endl;    //print numbers to screen
		float pos[3];
		float eigval[3];
		float eigvec[3];
		float radius;

		float id;
		ss >> id;

		ss >> pos[0];
		ss >> pos[1];
		ss >> pos[2];

		ss >> radius;

		ss >> eigval[0];
		ss >> eigval[1];
		ss >> eigval[2];

		ss >> eigvec[0];
		ss >> eigvec[1];
		ss >> eigvec[2];
		float3 e0 = make_float3(eigvec[0], eigvec[1], eigvec[2]);
		
		ss >> eigvec[0];
		ss >> eigvec[1];
		ss >> eigvec[2];
		float3 e1 = make_float3(eigvec[0], eigvec[1], eigvec[2]);

		ss >> eigvec[0];
		ss >> eigvec[1];
		ss >> eigvec[2];
		float3 e2 = make_float3(eigvec[0], eigvec[1], eigvec[2]);

		int nBin = cubemap_size * cubemap_size * 6;
		float *cubemap = new float[nBin];

		for (int i = 0; i < nBin; i++)	{
			ss >> cubemap[i];
		}
		
		float3 eigvec3 = make_float3(eigvec[0], eigvec[1], eigvec[2]);
		Halo * haloRecord = new Halo(
			(int)id, pos[0], pos[1], pos[2],
			radius,
			make_float3(eigval[0], eigval[1], eigval[2]),
			e0, e1, e2,
			cubemap, cubemap_size
			);

		/*
		switch ((int)id)
		{
		case 257:
			haloRecord->id = id = 679582;
			break;
		case 259:
			haloRecord->id = id = 671442;
			break;
		case 260:
			haloRecord->id = id = 663023;
			break;
		case 263:
			haloRecord->id = id = 671443;
			break;
		case 265:
			haloRecord->id = id = 671445;
			break;
		case 129:
			haloRecord->id = id = 663025;
			break;
		case 131:
			haloRecord->id = id = 663027;
			break;
		case 132:
			haloRecord->id = id = 654651;
			break;
		case 133:
			haloRecord->id = id = 654652;
			break;

		}
		*/

		halos.push_back(haloRecord);
		haloTable[id] = haloRecord;

		//if (haloTable.find(haloRecord->id) == haloTable.end()) {
		//	cout << "Clicked halo id " << id << " is not currently loaded in the timestep data" << endl;
		//}

		std::getline(fin, line);
//		fin >> data;               //get next number from file
	}
	fin.close();       //close file
	//assert(!fin.fail());

}

inline bool readNextToken(std::istream &in, int &token, bool &isNumber)
{
	//char c;
	//isNumber = false;

	//if (in.eof()) return false;

	//// skipping the space
	//while ((c = in.get()) == ' ')
	//	if (in.eof()) return false;

	//if (c >= '0' && c > token)
	//	isNumber = true;
	in >> token;
	if (in.eof()) {
		isNumber = false;
		return isNumber;
	}
	//cout << token;
	isNumber = (token != -1);
	return isNumber;
}


//This method recursively reads the contents of a merge tree listed from an in-order traversal
//The input is a preprocessed version of the merge tree data from the Dark Sky data
MergeNode * DataMgrCosm::readMergeTree(ifstream &fin, int treeId)
{
	int haloId = 0, numChildren = 0;  //Halo id and number of children for current node
	bool isNumber = true; //True if entry read is a number

	if (!readNextToken(fin, haloId, isNumber)) {
		cerr << "Did not have data to load as expected for halo id in readMergeTree method" << endl;
		return nullptr;
	}

	if (!readNextToken(fin, numChildren, isNumber)) {
		cerr << "Did not have data to load as expected for num children in readMergeTree method" << endl;
		return nullptr;
	}

	//Construct the current node
	MergeNode * currentNode = new MergeNode(); //TO DO: Destructor
	currentNode->haloId = haloId;

	mergeTreeTable[haloId] = currentNode;

	//Look for the halo id in the hashtable and record a reference if found
	if (haloTable.find(haloId) == haloTable.end()) {
		//cerr << "In method DataMgrCosm::readMergeTree " << haloId << " is not found" << endl;
	}
	else {
		cout << "Halo id " << haloId << " successfully loaded for tree id " << treeId << endl;
		currentNode->haloRecord = haloTable[haloId];	
	}

	currentNode->children.resize(numChildren);

	//Construct the children recursively
	for (int i = 0; i < numChildren; i++) {
		currentNode->children[i] = readMergeTree(fin, treeId);
	}

	return currentNode;

}



void DataMgrCosm::LoadMergeTree()
{
	//GetStringVal("vectorfield").c_str()
	//std::ifstream fMergeTreeIn(filenames["mergerTree"], std::fstream::in);  //Prprocessed Merge Tree data from Dark Sky data
	std::ifstream fMergeTreeIn(GetStringVal("mergerTree").c_str(), std::fstream::in);  //Prprocessed Merge Tree data from Dark Sky data

	if (!fMergeTreeIn)
	{
		cerr << "Merge Tree Input file did not load succedssfully" << endl;
		return;
	}

	int treeId = 0;  //Id of current merge tree read from preprocessed Dark Sky data
	bool isNumber = true; //True if current record was not -1 and did not reach end of file

	//Iterate through each merge tree in the preprocessed data
	while (readNextToken(fMergeTreeIn, treeId, isNumber)) {
		//cout << "Reading tree structure for tree id " << treeId << endl;
		//Read the current merge tree from the file
		MergeNode * root = readMergeTree(fMergeTreeIn, treeId);

		MergeTree * mergeTree = new MergeTree;
		mergeTree->treeId = treeId;
		mergeTree->root = root;
		//TO DO: Destructor

		//Add the new tree to the forest.  This push_back doesn't happen too often, but we still might consider storing the number of trees in the file.
		forest.push_back(mergeTree);
	}



	cout << "The Merge Tree Forest has been read" << endl;

}

//This method gets the MergeTree JSon for a given tree
//Parameter treeId - the id of the tree for which we should get JSon
//JSon example:
//{
//"1" :  
//[{"3" : [null]}],
//"2" : 
//[{"4" : [null]}],
//};
QString DataMgrCosm::getMergeTreeJSon(int treeId)
{
	QString output = buildJsonFromTree(forest[treeId]->root, 0);
	//cout << output.toStdString() << endl;
	return output;
}

//This method recursively reads the contents of a merge tree listed from an in-order traversal
//The input is a preprocessed version of the merge tree data from the Dark Sky data
QString DataMgrCosm::buildJsonFromTree(MergeNode * currentNode, int level)
{
	const int DEPTH_LIMIT = 4;

	int haloId = currentNode->haloId;  //Halo id for current node

	QString currentString = QString("{ \"") + QString::number(haloId) + QString("\" : [");

	//Construct the children recursively
	if (currentNode->children.size() == 0 || level > DEPTH_LIMIT) {
		currentString += QString("null");
	}
	else {
		for (int i = 0; i < currentNode->children.size(); i++) {
			currentString += buildJsonFromTree(currentNode->children[i], level + 1);
			if (i != currentNode->children.size() - 1) {
				currentString += ",";
			}
		}
	}

	currentString += QString("] }");


	return currentString;
}

//Recursively set the visibility for all the current merge tree node and all its children
void DataMgrCosm::SetChildrenVisibility(MergeNode *nd, bool _isVisible)
{
	if (nd != nullptr) {
		//Store the visibility in the merge tree node (Some halos may not be loaded)
		nd->isVisible = _isVisible;

		//Store the visibility in the halo itself if it is loaded (used during rendering)
		AbstractNode * haloRecord = nd->haloRecord;
		if (haloRecord != nullptr) {
			haloRecord->SetVisible(_isVisible);
		}

		//Handle visibility for child nodes in the merge tree
		for (int i = 0; i < nd->children.size(); i++) {
			SetChildrenVisibility(nd->children[i], _isVisible);
		}
	}
}
