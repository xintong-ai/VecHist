#ifndef GraphVizWidgetCPP
#define GraphVizWidgetCPP

#include <QWidget>
#include <fstream>

using namespace std;

class GraphVizWidget : public QWidget
{
private:
	ifstream inFile;
public:
	void loadGraphVizTextFile();

};

#endif