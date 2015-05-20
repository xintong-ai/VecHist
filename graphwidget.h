/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
//#include <DataManager.h>
#include <DataMgrVect.h>
#include <node.h>
#include <vtkLookupTable.h>

class Widget::Node;

struct GraphVizNode
{
	string name;
	double x;  //X coordinate
	double y;  //U coordinate
	double width; //Ellipse width
	double height; //Ellipse height
	Widget::Node * widgetNode = nullptr;  //QT Widget node
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

class GraphWidget : public QGraphicsView
{
	//Q_OBJECT

public:
	GraphWidget(vtkLookupTable * colorTable, QWidget *parent = 0, NodeBi *p = 0);
	~GraphWidget();
	
	void buildDotFileFromTree(NodeBi * root);
	void buildDotFileFromTree(NodeBi * p, int currentDepth, int previousId);
	void buildPlainTextFileFromDot();
	void loadGraphVizTextFile();
	void getTreeStats(NodeBi * p, int currentDepth, int currentPos);
	Widget::Node * buildGraphFromTree(NodeBi * p);
	Widget::Node * buildGraphFromTree(NodeBi * p, int currentDepth, int currentPos, double x, double y);

	void itemMoved();

	public slots:
	void shuffle();
	void zoomIn();
	void zoomOut();

protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
	void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

	void scaleView(qreal scaleFactor);

private:
	int timerId;
	Widget::Node *centerNode;

	int maxTreeDepth = 0;
	int minPos = 0;
	int maxPos = 0;

	/////////////////////////
	ifstream inFile;							 //Output file from dot to be read
	double graphScale, graphWidth, graphHeight;  //Attributes about entire graph that will be needed to establish the coordinate system
	unordered_map<string, GraphVizNode *> nodeTable; //HashTable linking node ids to the struct records
	unordered_map<string, NodeBi *> nodeBiTable;     //HashTable linking node ids to NodeBi struct records
	vector<GraphVizNode *> nodes;					//The list of all nodes
	vector<GraphVizEdge *> edges;					//The list of all edges
	ofstream dotOut;								//The output file stream handler for the file that will be read by the dot program
	vtkLookupTable * colorTable;					//Reference to the VTK color table
	
};

#endif // GRAPHWIDGET_H