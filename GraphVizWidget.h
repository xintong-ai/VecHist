#ifndef GraphVizWidgetCPP
#define GraphVizWidgetCPP

#include <QWidget>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

struct GraphVizNode
{
	string name;
	double x;  //X coordinate
	double y;  //U coordinate
	double width; //Ellipse width
	double height; //Ellipse height
};

struct ControlPoint
{
	double x;
	double y;
};

struct GraphVizEdge
{
	GraphVizNode * head; //Pointer to GraphVizNode representing the head
	GraphVizNode * tail; //Pointer to GraphVizNode representing the head
	vector<ControlPoint> controlPoints; //Set of control points representing the b-spline for the edge's curve

};

class GraphVizWidget : public QWidget
{
private:
	ifstream inFile;							 //Output file from dot to be read
	double graphScale, graphWidth, graphHeight;  //Attributes about entire graph that will be needed to establish the coordinate system
	unordered_map<string, GraphVizNode *> nodeTable; //HashTable linking node ids to the struct records
	vector<GraphVizNode *> nodes;					//The list of all nodes
	vector<GraphVizEdge *> edges;					//The list of all edges
public:
	~GraphVizWidget();
	void loadGraphVizTextFile();

};

#endif