#include <iostream>

#include "GraphVizWidget.h"


void GraphVizWidget::loadGraphVizTextFile()
{
	inFile.open("tree.ptxt", ios::in);
	if (!inFile.is_open())
	{
		cerr << "tree.ptxt did not open successfully" << endl;
		return;
	}

	//Specs from http://www.graphviz.org/doc/info/output.html#d:plain-ext
	//There are four types of statements.
	//graph scale width height
	//node name x y width height label style shape color fillcolor
	//edge tail head n x1 y1 ..xn yn[label xl yl] style color
	//stop

	double graphScale, graphWidth, graphHeight;  //Attributes about entire graph that will be needed to establish the coordinate system

	string token; //The general token currently read from the file needing to be processed
	string name; //The name of the node currently being processed
	string headName; //The name of the head element of an edge currently being processed
	string tailName; //The name of the tail element of an edge currently being processed
	int numNodes;    //The number of nodes constituting one b-spline
	double x, y; //The x and y coordinates of the node/edge being read
	double width, height;  //The width and heigh of the node currently being read

	while (!inFile.fail()) {
		inFile >> token;
		if (token == "graph") {
			cout << "New graph record: " << endl;
			inFile >> graphScale;
			inFile >> graphWidth;
			inFile >> graphHeight;
			cout << "Scale: " << graphScale << " Width: " << graphWidth << " Height: " << graphHeight << endl;
		}
		else if (token == "node") {
			cout << "New node record: " << endl;
			inFile >> name;
			cout << "Name: " << name << endl;
			inFile >> x >> y;
			cout << "x = " << x << " y = " << y << endl;
			inFile >> width >> height;
			cout << "width = " << width << " height = " << height << endl;
			cout << "Unused tokens: " << endl;
			for (int i = 0; i < 5; i++) {
				inFile >> token;
				cout << token << " ";
			}
			cout << endl;
		}
		else if (token == "edge") {
			cout << "New edge record:" << endl;
			inFile >> headName;
			cout << "Head Name: " << headName << endl;
			inFile >> tailName;
			cout << "Tail Name: " << tailName << endl;
			inFile >> numNodes;
			cout << "Num Nodes: " << numNodes << endl;
			for (int i = 0; i < numNodes; i++) {
				inFile >> x;
				inFile >> y;
				cout << "Control Point # " << i << ": " << x << " " << y << endl;
			}
			cout << endl;
			cout << "Unused tokens: " << endl;
			for (int i = 0; i < 2; i++) {
				inFile >> token;
				cout << token << " ";
			}
			cout << endl;
		}
		else if (token == "stop") {
			cout << "Stop request encountered" << endl;
		}
		else {
			cerr << "Warning: unexpected token " << token << " found in plain text dot" << endl;
		}
	}

	cout << "-----------------------------" << endl;
	cout << "End of file reached" << endl;
	cout << "-----------------------------" << endl;

	cout << endl;

	
}
