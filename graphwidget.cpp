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

#include "graphwidget.h"
#include "edge.h"
#include "node.h"

#include <math.h>

#include <QKeyEvent>

GraphWidget::GraphWidget(QWidget *parent, NodeBi *p)
	: QGraphicsView(parent), timerId(0)
{
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	//scene->setSceneRect(0, 0, 800, 800);
	scene->setSceneRect(0, 0, 800 * 4, 800 * 2);
	setScene(scene);
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	scale(qreal(0.8), qreal(0.8));
	setMinimumSize(400, 400);
	//setMinimumSize(100, 100);
	setMaximumSize(800, 800);
	setWindowTitle(tr("Tree Widget"));

	if (p != nullptr) {
		//getTreeStats(p, 0, 0);
		//buildGraphFromTree(p);
	}

}

GraphWidget::~GraphWidget()
{
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}

	for (int i = 0; i < edges.size(); i++) {
		delete edges[i];
	}

}

void GraphWidget::loadGraphVizTextFile()
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

	string token; //The general token currently read from the file needing to be processed
	string name; //The name of the node currently being processed
	string headName; //The name of the head element of an edge currently being processed
	string tailName; //The name of the tail element of an edge currently being processed
	int numNodes;    //The number of nodes constituting one b-spline
	double x, y; //The x and y coordinates of the node/edge being read
	double width, height;  //The width and height of the node currently being read

	nodes.clear();

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
			//Parse the Node Record
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

			//Store the node record
			GraphVizNode * node = new GraphVizNode;
			node->name = name;
			node->x = x;
			node->y = y;
			node->width = width;
			node->height = height;

			nodes.push_back(node);

			nodeTable[name] = node;


		}
		else if (token == "edge") {
			//////////////Parse the main part of the edge record//////////////////
			cout << "New edge record:" << endl;
			inFile >> headName;
			cout << "Head Name: " << headName << endl;
			inFile >> tailName;
			cout << "Tail Name: " << tailName << endl;
			inFile >> numNodes;
			cout << "Num Nodes: " << numNodes << endl;

			///////////Store the edge record///////////////////////
			GraphVizNode * headPtr = nodeTable[headName];
			if (headPtr == nullptr) {
				cerr << headName << " not found!!!!!" << endl;
				break;
			}

			GraphVizNode * tailPtr = nodeTable[tailName];
			if (tailPtr == nullptr) {
				cerr << tailName << " not found!!!!!" << endl;
				break;
			}

			GraphVizEdge * edge = new GraphVizEdge();
			edge->head = headPtr;
			edge->tail = tailPtr;
			edge->controlPoints.resize(numNodes);


			//////////Parse and store the control point records////
			for (int i = 0; i < numNodes; i++) {
				inFile >> x;
				inFile >> y;
				cout << "Control Point # " << i << ": " << x << " " << y << endl;

				ControlPoint controlPoint;
				controlPoint.x = x;
				controlPoint.y = y;
				edge->controlPoints.push_back(controlPoint);
			}

			//Add the new edge to the list
			edges.push_back(edge);

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

	/////////////////////////////////////////////////////////////
	//Reference code:
	/*
	Widget::Node *currentNode = new Widget::Node(this);
	currentNode->setNodeBiPtr(p);
	p->SetGraphNode(currentNode);

	currentNode->setPos(x, y);
	scene()->addItem(currentNode);

	//Use the maximum depth and maximum width of the tree to set the amount of space that each tree node takes up
	double nodeHeight = sceneRect().height() / double(maxTreeDepth) * 0.975;  //97.5% to compensate for slight y shift mentioned in launcher method above
	double nodeWidth = sceneRect().width() / 2;

	//Recurse to the next level in the tree, and then add a new graph edge for the child node
	Widget::Node *childNode = nullptr;
	if (p->GetLeft() != nullptr) {
	childNode = buildGraphFromTree(p->GetLeft(), currentDepth + 1, currentPos - 1, x - nodeWidth * (pow(0.5,currentDepth + 1)), y + nodeHeight); //The distance between nodes is cut to fraction 0.5 ^ (currentDepth + 1) to prevent node overlap
	scene()->addItem(new Edge(currentNode, childNode));
	}
	if (p->GetRight() != nullptr) {
	childNode = buildGraphFromTree(p->GetRight(), currentDepth + 1, currentPos + 1, x + nodeWidth * (pow(0.5,currentDepth + 1)), y + nodeHeight);
	scene()->addItem(new Edge(currentNode, childNode));
	}

	return currentNode;
	*/

	scene()->setSceneRect(0, 0, graphWidth, graphHeight);

	for (int i = 0; i < nodes.size(); i++) {
		Widget::Node * childNode = new Widget::Node(this);
		childNode->setPos(nodes[i]->x, nodes[i]->y);
		childNode->RADIUS = nodes[i]->width / 2;
		childNode->RADIUS = 0.00000001;
		scene()->addItem(childNode);
		//childNode->set

	}

	/*
	for (int i = 0; i < edges.size(); i++) {

		Edge * edge = new Edge();
	}
	*/
}

//Method to find max tree depth and max tree "width"
void GraphWidget::getTreeStats(NodeBi * p, int currentDepth, int currentPos)
{
	//Manage tree statistics
	if (currentDepth > maxTreeDepth)
	{
		maxTreeDepth = currentDepth;
	}

	if (currentPos < minPos)
	{
		minPos = currentPos;
	}
	if (currentPos > maxPos)
	{
		maxPos = currentPos;
	}

	//Recurse
	if (p->GetLeft() != nullptr) {
		getTreeStats(p->GetLeft(), currentDepth + 1, currentPos - 1);
		
	}
	if (p->GetRight() != nullptr) {
		getTreeStats(p->GetRight(), currentDepth + 1, currentPos + 1);
		
	}
}

//This method launches the recursive graph building method
Widget::Node * GraphWidget::buildGraphFromTree(NodeBi * p)
{
	return buildGraphFromTree(p, 0, 0, sceneRect().width() / 2, 10); //Shove down by 10 units - for some reason the topmost node was slightly out of the scene rectangle
}

//This method builds the displayed graph from the tree data structure, using recursion to do an in order traversal
Widget::Node * GraphWidget::buildGraphFromTree(NodeBi * p, int currentDepth, int currentPos, double x, double y)
{
	Widget::Node *currentNode = new Widget::Node(this);
	currentNode->setNodeBiPtr(p);
	p->SetGraphNode(currentNode);
	
	currentNode->setPos(x, y);
	scene()->addItem(currentNode);

	//Use the maximum depth and maximum width of the tree to set the amount of space that each tree node takes up
	double nodeHeight = sceneRect().height() / double(maxTreeDepth) * 0.975;  //97.5% to compensate for slight y shift mentioned in launcher method above
	double nodeWidth = sceneRect().width() / 2;

	//Recurse to the next level in the tree, and then add a new graph edge for the child node
	Widget::Node *childNode = nullptr;
	if (p->GetLeft() != nullptr) {
		childNode = buildGraphFromTree(p->GetLeft(), currentDepth + 1, currentPos - 1, x - nodeWidth * (pow(0.5,currentDepth + 1)), y + nodeHeight); //The distance between nodes is cut to fraction 0.5 ^ (currentDepth + 1) to prevent node overlap
		scene()->addItem(new Edge(currentNode, childNode));
	}
	if (p->GetRight() != nullptr) {
		childNode = buildGraphFromTree(p->GetRight(), currentDepth + 1, currentPos + 1, x + nodeWidth * (pow(0.5,currentDepth + 1)), y + nodeHeight);
		scene()->addItem(new Edge(currentNode, childNode));
	}

	return currentNode;
}

void GraphWidget::itemMoved()
{
	if (!timerId)
		timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		centerNode->moveBy(0, -20);
		break;
	case Qt::Key_Down:
		centerNode->moveBy(0, 20);
		break;
	case Qt::Key_Left:
		centerNode->moveBy(-20, 0);
		break;
	case Qt::Key_Right:
		centerNode->moveBy(20, 0);
		break;
	case Qt::Key_Plus:
		zoomIn();
		break;
	case Qt::Key_Minus:
		zoomOut();
		break;
	case Qt::Key_Space:
	case Qt::Key_Enter:
		shuffle();
		break;
	default:
		QGraphicsView::keyPressEvent(event);
	}
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);

	QList<Widget::Node *> nodes;
	foreach(QGraphicsItem *item, scene()->items()) {
		if (Widget::Node *node = qgraphicsitem_cast<Widget::Node *>(item))
			nodes << node;
	}

	foreach(Widget::Node *node, nodes)
		node->calculateForces();

	bool itemsMoved = false;
	foreach(Widget::Node *node, nodes) {
		if (node->advance())
			itemsMoved = true;
	}

	if (!itemsMoved) {
		killTimer(timerId);
		timerId = 0;
	}
}

#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
	scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
	Q_UNUSED(rect);

	// Shadow
	QRectF sceneRect = this->sceneRect();
	QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
	QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
		painter->fillRect(rightShadow, Qt::darkGray);
	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
		painter->fillRect(bottomShadow, Qt::darkGray);

	// Fill
	QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
	gradient.setColorAt(0, Qt::white);
	gradient.setColorAt(1, Qt::lightGray);
	painter->fillRect(rect.intersected(sceneRect), gradient);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(sceneRect);

	// Text
	QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
		sceneRect.width() - 4, sceneRect.height() - 4);
	QString message(tr(""));

	QFont font = painter->font();
	font.setBold(true);
	font.setPointSize(14);
	painter->setFont(font);
	painter->setPen(Qt::lightGray);
	painter->drawText(textRect.translated(2, 2), message);
	painter->setPen(Qt::black);
	painter->drawText(textRect, message);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
	foreach(QGraphicsItem *item, scene()->items()) {
		if (qgraphicsitem_cast<Widget::Node *>(item))
			item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
	}
}

void GraphWidget::zoomIn()
{
	scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
	scaleView(1 / qreal(1.2));
}