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

//using Widget::Node;

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
		getTreeStats(p, 0, 0);
		buildGraphFromTree(p);
	}

	/*
	Widget::Node *node1 = new Widget::Node(this);
	Widget::Node *node2 = new Widget::Node(this);
	Widget::Node *node3 = new Widget::Node(this);
	Widget::Node *node4 = new Widget::Node(this);
	centerNode = new Widget::Node(this);
	Widget::Node *node6 = new Widget::Node(this);
	Widget::Node *node7 = new Widget::Node(this);
	Widget::Node *node8 = new Widget::Node(this);
	Widget::Node *node9 = new Widget::Node(this);
	scene->addItem(node1);
	scene->addItem(node2);
	scene->addItem(node3);
	scene->addItem(node4);
	scene->addItem(centerNode);
	scene->addItem(node6);
	scene->addItem(node7);
	scene->addItem(node8);
	scene->addItem(node9);
	scene->addItem(new Edge(node1, node2));
	scene->addItem(new Edge(node2, node3));
	scene->addItem(new Edge(node2, centerNode));
	scene->addItem(new Edge(node3, node6));
	scene->addItem(new Edge(node4, node1));
	scene->addItem(new Edge(node4, centerNode));
	scene->addItem(new Edge(centerNode, node6));
	scene->addItem(new Edge(centerNode, node8));
	scene->addItem(new Edge(node6, node9));
	scene->addItem(new Edge(node7, node4));
	scene->addItem(new Edge(node8, node7));
	scene->addItem(new Edge(node9, node8));

	node1->setPos(-50, -50);
	node2->setPos(0, -50);
	node3->setPos(50, -50);
	node4->setPos(-50, 0);
	centerNode->setPos(0, 0);
	node6->setPos(50, 0);
	node7->setPos(-50, 50);
	node8->setPos(0, 50);
	node9->setPos(50, 50);
	*/
}

//Method to find max tree depth and max tree "width"
void GraphWidget::getTreeStats(NodeBi * p, int currentDepth, int currentPos)
{
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

	if (p->left != nullptr) {
		getTreeStats(p->left, currentDepth + 1, currentPos - 1);
		
	}
	if (p->right != nullptr) {
		getTreeStats(p->right, currentDepth + 1, currentPos + 1);
		
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
	
	currentNode->setPos(x, y);
	scene()->addItem(currentNode);

	double nodeWidth = sceneRect().width() / double(abs(minPos) + maxPos) * 4;
	double nodeHeight = sceneRect().height() / double(maxTreeDepth) * 0.975;  //97.5% to compensate for slight y shift mentioned in launcher method above

	//cout << nodeWidth << " " << nodeHeight << endl;

	Widget::Node *childNode = nullptr;
	if (p->left != nullptr) {
		childNode = buildGraphFromTree(p->left, currentDepth + 1, currentPos - 1, x - nodeWidth * (pow(0.5,currentDepth + 1)), y + nodeHeight); //The distance between nodes is cut to fraction 0.5 ^ (currentDepth + 1) to prevent node overlap
		scene()->addItem(new Edge(currentNode, childNode));
	}
	if (p->right != nullptr) {
		childNode = buildGraphFromTree(p->right, currentDepth + 1, currentPos + 1, x + nodeWidth * (pow(0.5,currentDepth + 1)), y + nodeHeight);
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