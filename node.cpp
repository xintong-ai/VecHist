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

#include "edge.h"
#include "node.h"
#include "graphwidget.h"
#include "TreeMapWindow.h"
#include "TextureCubeManager.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

//Node constructor
Widget::Node::Node(GraphWidget *graphWidget, TextureCubeManager * textureCubeManager)
	: graph(graphWidget)
{
	this->textureCubeManager = textureCubeManager;
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
	setZValue(-1);
	nodeBiPtr = nullptr;
}

void Widget::Node::addEdge(Edge *edge)
{
	edgeList << edge;
	edge->adjust();
}

QList<Edge *> Widget::Node::edges() const
{
	return edgeList;
}

void Widget::Node::calculateForces()
{
	if (!scene() || scene()->mouseGrabberItem() == this) {
		newPos = pos();
		return;
	}

	// Sum up all forces pushing this item away
	qreal xvel = 0;
	qreal yvel = 0;
	foreach(QGraphicsItem *item, scene()->items()) {
		Widget::Node *node = qgraphicsitem_cast<Widget::Node *>(item);
		if (!node)
			continue;

		QPointF vec = mapToItem(node, 0, 0);
		qreal dx = vec.x();
		qreal dy = vec.y();
		double l = 2.0 * (dx * dx + dy * dy);
		if (l > 0) {
			xvel += (dx * 150.0) / l;
			yvel += (dy * 150.0) / l;
		}
	}

	// Now subtract all forces pulling items together
	double weight = (edgeList.size() + 1) * 10;
	foreach(Edge *edge, edgeList) {
		QPointF vec;
		if (edge->sourceNode() == this)
			vec = mapToItem(edge->destNode(), 0, 0);
		else
			vec = mapToItem(edge->sourceNode(), 0, 0);
		xvel -= vec.x() / weight;
		yvel -= vec.y() / weight;
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
		xvel = yvel = 0;

	QRectF sceneRect = scene()->sceneRect();
	newPos = pos() + QPointF(xvel, yvel);
	newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
	newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

//This method causes a node to advance its state based on the physics
//The logic for this is currently DISABLED
bool Widget::Node::advance()
{
	if (newPos == pos())
		return false;

	//This is commented out for now - we have disabled movement physics
	//setPos(newPos);
	return true;
}

QRectF Widget::Node::boundingRect() const
{
	qreal adjust = 2;
	return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Widget::Node::shape() const
{
	QPainterPath path;
	path.addEllipse(-10, -10, 20, 20);
	return path;
}

//This is the QT Paint method for graph nodes
void Widget::Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	//Currently only a radius at a maximum of 10 is guaranteed to work correctly, since the bounding box of the whole node is not large enough
	//We will need to look into howt to increase it later
	//The shadow does not work correctly yet for smaller radii
	//This component was very much hardwired for a radius of 10 originally
	const int RADIUS = 10;

	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGray);
	painter->drawEllipse(-RADIUS * 0.7, -RADIUS * 0.7, RADIUS, RADIUS);

	//Set the colors for the interior
	QColor foreColor;
	QColor backColor;
	if (!nodeBiPtr->GetSelected())
	{
		foreColor = foreDisplayColor;
		backColor = backDisplayColor;

	}
	else
	{
		foreColor = Qt::magenta;
		backColor = Qt::darkMagenta;

	}	

	//Draw the interior
	QRadialGradient gradient(-3, -3, 10);
	if (option->state & QStyle::State_Sunken) {
		gradient.setCenter(3, 3);
		gradient.setFocalPoint(3, 3);
		gradient.setColorAt(1, foreColor.light(120));
		gradient.setColorAt(0, backColor.light(120));
	}
	else {
		gradient.setColorAt(0, foreColor);
		gradient.setColorAt(1, backColor);
	}
	painter->setBrush(gradient);

	//Draw the border
	painter->setPen(QPen(Qt::black, 0));
	painter->drawEllipse(-RADIUS, -RADIUS, RADIUS, RADIUS);
	
	//Draw X's for invisible nodes
	if (!nodeBiPtr->GetVisible()) {
		painter->setPen(QPen(Qt::darkBlue, 2.5));
		painter->drawLine(-RADIUS, -RADIUS, 0, 0);
		painter->drawLine(-RADIUS, 0, 0, -RADIUS);
	}
}

QVariant Widget::Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		foreach(Edge *edge, edgeList)
			edge->adjust();
		graph->itemMoved();
		break;
	default:
		break;
	};

	return QGraphicsItem::itemChange(change, value);
}

//Mouse press event for a node
//Toggles the selected state for a node
//Also prints information about it to the console - useful for debugging
void Widget::Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mousePressEvent(event);
	//Toggle visibility
	//nodeBiPtr->SetVisible( !nodeBiPtr->GetVisible());
	nodeBiPtr->SetSelected(!nodeBiPtr->GetSelected());
	//SetChildrenVisibility(nodeBiPtr, nodeBiPtr->GetVisible());

	NodeBi * nodeBiPtr = getNodeBiPtr();
	double entropyValue = nodeBiPtr->GetEntropy();
	int volumeValue = nodeBiPtr->GetVolume();
	nodeBiPtr->getTreeMapWindow()->updateChildren();
	setToolTip(QString("Entropy: ") + QString::number(entropyValue));
	cout << "Entropy of Node: " << entropyValue << endl;
	cout << "Volume of Node: " << volumeValue << endl;
	cout << "Name of Node: " << name << endl;
}

void Widget::Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	update();
	QGraphicsItem::contextMenuEvent(event);

	NodeBi * nodeBiPtr = getNodeBiPtr();
	graph->splitSuperQuadric(nodeBiPtr);
	//TODO: Externalize setting of application (1) for this
	textureCubeManager->UpdateTexture(1);

}

void Widget::Node::SetChildrenVisibility(NodeBi *nd, bool _isVisible)
{
	nd->SetVisible(_isVisible);

	if (nd->GetGraphNode() != nullptr) {
		nd->GetGraphNode()->update();
	}

	if (nd->GetLeft() != nullptr) {
		SetChildrenVisibility(nd->GetLeft(), _isVisible);
	}
	if (nd->GetRight() != nullptr) {
		SetChildrenVisibility(nd->GetRight(), _isVisible);
	}


}

void Widget::Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}

/*void Widget::Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	NodeBi * nodeBiPtr = getNodeBiPtr();
	double entropyValue = nodeBiPtr->GetEntropy();
	setToolTip(QString("Entropy: ") + QString::number(entropyValue));


}*/