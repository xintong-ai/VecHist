/*
 * Copyright (c) 2010 Mark Liversedge (liversedge@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "DataManager.h"
#include "DataMgrVect.h"

#include "TreeMapPlot.h"
#include "TreeMapWindow.h"

#include <QSettings>
#include <cmath> // for isinf() isnan()
#include <vector>
#include <iostream>

using namespace std;

bool TreeMapLessThan(const TreeMap *a, const TreeMap *b) {
	return (a->value) > (b->value);
}

//Constructor
//Parameter parent - the TreeMapWindow object to which the tree map belongs
TreeMapPlot::TreeMapPlot(TreeMapWindow *parent, DataManager * dataManager)
	: QWidget(parent), parent(parent)
{
	this->dataManager = dataManager;
	root = new TreeMap;
	setMouseTracking(true);
	installEventFilter(this);
    
}

//Destructor
TreeMapPlot::~TreeMapPlot()
{
}

//Sets the nodeBi root record and causes the display tree and layout to be generated
void TreeMapPlot::setData(NodeBi * rootBi)
{
	this->rootBi = rootBi;

	//Issue a resize event
	//This will cause a new layout to be issued,
	//which will both build the display tree from the root Bi node and
	//then run the necessary layout algorithms
	resizeEvent(NULL);
}

//Builds the display tree used by the layout system
void TreeMapPlot::buildTree() 
{
	if (root != nullptr) {
		root->clear();
	}

	switch (layoutMethod) {
	case 0:
	case 1:
		buldMultiLevelTree(rootBi);
		break;
	case 2:
		buildTreeOfLeaves(rootBi);
		break;
	}
	
	leafNodes.clear();
	buildLeafList(root);


}

//Builds a display tree with multiple node levels
//Parameter biNode - the root node of the tree
void TreeMapPlot::buldMultiLevelTree(NodeBi * biNode)
{
	//Deal with edge cases
	if (biNode == nullptr) {
		root->insert(QString(""), 0);
	}
	else if (biNode->GetLeft() == nullptr && biNode->GetRight() == nullptr) {
		biNode->setTreeMapWindow(parent);
		root->insert(QString(""), biNode->GetEntropy());
	}
	else {
		biNode->setTreeMapWindow(parent);
		//Recurse to the next level in the tree
		if (biNode->GetLeft() != nullptr) {
			buldMultiLevelTree(biNode->GetLeft(), root, 0);
		}
		if (biNode->GetRight() != nullptr) {
			buldMultiLevelTree(biNode->GetRight(), root, 0);
		}
	}

}

//Recursively builds the remainder of the display tree, building a tree with multiple node levels
//Parameter biNode - current BI node data record
//Parameter treeMapNode - current display tree record
//Current depth - how deep we are into the tree
void TreeMapPlot::buldMultiLevelTree(NodeBi * biNode, TreeMap * treeMapNode, int currentDepth)
{
	//Record the tree window reference for each node
	//Hopefully we will replace this later with references to individual tree map nodes, once we resolve the circular #include design issues
	biNode->setTreeMapWindow(parent);

	TreeMap * newNode = nullptr;

	const int DEPTH_LIMIT = 100000; //This constant can be used to limit the number of levels used - it is useful in debugging.  For now it is set to a massive number to make there be no real limit.

	//If we are at a leaf node, add a regular value to the tree map
	if (biNode->GetLeft() == nullptr && biNode->GetRight() == nullptr || currentDepth >= DEPTH_LIMIT) {
		//newNode = treeMapNode->insert(QString(""), biNode->GetEntropy());
		newNode = treeMapNode->insert(QString::number(biNode->GetEntropy(), 'g', 2), biNode->GetEntropy());
	}
	//Otherwise we are not at a leaf node, in which case the value really doesn't matter.  Show that with a value of 0.
	else {
		newNode = treeMapNode->insert(QString(""), 0);
	}

	newNode->nodeBiRef = biNode;

	//Recurse to the next level in the tree
	if (biNode->GetLeft() != nullptr && currentDepth < DEPTH_LIMIT) {
		buldMultiLevelTree(biNode->GetLeft(), newNode, currentDepth + 1); 
	}
	if (biNode->GetRight() != nullptr && currentDepth < DEPTH_LIMIT) {
		buldMultiLevelTree(biNode->GetRight(), newNode, currentDepth + 1);
	}


}

//Builds a display tree in which only leaf nodes from the original BI tree are added, all under a comon root.  Thus this tree only has "one level" under the root.
//Parameter biNode - the root NodeBi record from which to build
void TreeMapPlot::buildTreeOfLeaves(NodeBi * biNode)
{
	//Deal with edge cases
	if (biNode == nullptr) {
		root->insert(QString(""), 0);
	}
	else if (biNode->GetLeft() == nullptr && biNode->GetRight() == nullptr) {
		biNode->setTreeMapWindow(parent);
		root->insert(QString(""), biNode->GetEntropy());
	}
	else {
		biNode->setTreeMapWindow(parent);
		//Recurse to the next level in the tree
		if (biNode->GetLeft() != nullptr) {
			buildTreeOfLeaves(biNode->GetLeft(), root, 0);
		}
		if (biNode->GetRight() != nullptr) {
			buildTreeOfLeaves(biNode->GetRight(), root, 0);
		}
	}

}

//This method recursively builds the remainder of the display tree, building it only using leaf nodes from the original BI tree
//Parameter biNode - current BI node data record
//Parameter treeMapNode - current display tree record
//Current depth - how deep we are into the tree
void TreeMapPlot::buildTreeOfLeaves(NodeBi * biNode, TreeMap * treeMapNode, int currentDepth)
{
	//Record the tree window reference for each node
	//Hopefully we will replace this later with references to individual tree map nodes, once we resolve the circular #include design issues
	biNode->setTreeMapWindow(parent);

	TreeMap * newNode = nullptr;

	const int DEPTH_LIMIT = 100000; //This constant can be used to limit the number of levels used - it is useful in debugging.  For now it is set to a massive number to make there be no real limit.

	//If we are at a leaf node, add a regular value to the tree map
	if (biNode->GetLeft() == nullptr && biNode->GetRight() == nullptr || currentDepth >= DEPTH_LIMIT) {
		//newNode = treeMapNode->insert(QString(""), biNode->GetEntropy());
		newNode = root->insert(QString::number(biNode->GetEntropy(), 'g', 2), biNode->GetEntropy());
		newNode->nodeBiRef = biNode;
	}
	
	//Recurse to the next level in the tree
	if (biNode->GetLeft() != nullptr && currentDepth < DEPTH_LIMIT) {
		buildTreeOfLeaves(biNode->GetLeft(), newNode, currentDepth + 1);
	}
	if (biNode->GetRight() != nullptr && currentDepth < DEPTH_LIMIT) {
		buildTreeOfLeaves(biNode->GetRight(), newNode, currentDepth + 1);
	}


}

//This method responds to resize events, rebuilding the layout
void TreeMapPlot::resizeEvent(QResizeEvent *)
{
	issueLayout();
}

//This method paints to the QT widget
void TreeMapPlot::paintEvent(QPaintEvent *)
{
	TreeMap * drawNode = nullptr;

	drawNode = root;

	//areaReport();
	if (!drawNode) return;

	//resize(sizeHint() * 2);

	// labels
	

	// Init paint settings
	QPainter painter(this);

	QColor color = QColor(0, 0, 255, 255);
	QPen pen(color);
	pen.setWidth(10); // root
	QBrush brush(color);
	painter.setBrush(brush);
	painter.setPen(pen);

	// draw border and background (root node)
	painter.drawRect(drawNode->rect.x() + 4,
		drawNode->rect.y() + 4,
		drawNode->rect.width() - 8,
		drawNode->rect.height() - 8);

	// first level - rectangles, but not text yet
	pen.setWidth(5);
	pen.setColor(color);
	painter.setPen(pen);

	paintChildren(drawNode, painter, brush, 0);




}

//This method recursively paints the children of a display tree
//Parameter parent -- the current display tree node
//Parameter painter - the QPainter object being used
//Parameter brush - the Qbrush object being used
//Parameter level - how deep we are into the tree
void TreeMapPlot::paintChildren(TreeMap * parent, QPainter & painter, QBrush & brush, int level)
{
	QFont font;
	QPen textPen;
	QColor hcolor(Qt::lightGray);
	hcolor.setAlpha(127);
	QBrush hbrush(hcolor);

	QPen selectedPen;
	selectedPen.setColor(Qt::yellow);
	selectedPen.setWidth(5);

	textPen.setColor(Qt::darkMagenta);

	int fontSizeValue = 0;

	//Make the starting font size a bit smaller for some of the lower levels
	switch (level) {
	case 0:
		fontSizeValue = 18;
		break;
	case 1:
		fontSizeValue = 16;
		break;
	default:
		fontSizeValue = 14;
		break;
	}

	font.setPointSize(fontSizeValue);

	painter.setFont(font);

	int n = 1;
	QColor cHSV, cRGB;
	double factor = double(1) / double(parent->children.count());
	double color[3];

	foreach(TreeMap *first, parent->children) {

		//Use color map based on entropy value to determine color of rendered square
		double entropyValue = first->value;

		((DataMgrVect *)dataManager)->getEntropyColor(entropyValue, color);
		cRGB.setRgb(255 * color[0], 255 * color[1], 255 * color[2], 255);


		brush.setColor(cRGB);
		
		//Set the brush.  Use a hightlight if a node is moused over
		if (first == highlight)
			painter.setBrush(hbrush);
		else
			painter.setBrush(brush);
		
		//Set the pen.  Use the selection color if a node was clicked.
		if (first->nodeBiRef != nullptr && first->nodeBiRef->GetSelected())
			painter.setPen(selectedPen);
		else
			painter.setPen(textPen);

		const int OFFSET_FACTOR = 0;
		QRect drawRect;
		drawRect.setRect(first->rect.x() + level * OFFSET_FACTOR, first->rect.y() + level * OFFSET_FACTOR, first->rect.width() - level * OFFSET_FACTOR, first->rect.height() - level * OFFSET_FACTOR);

		painter.drawRect(drawRect);

		paintChildren(first, painter, brush, level + 1);

		QRect textRect;
		QRect demandedTextRect;
		textRect.setRect(first->rect.x() + 2, first->rect.y() + 2, first->rect.width() - 4, first->rect.height() - 4);

		painter.setFont(font);

		painter.setPen(textPen);

		demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, first->name);
		if (!textRect.contains(demandedTextRect)) {
			font.setPointSize(fontSizeValue - 2);
			painter.setFont(font);
			demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, first->name);
			if (!textRect.contains(demandedTextRect)) {
				font.setPointSize(fontSizeValue - 4);
				painter.setFont(font);
				demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, first->name);
				if (!textRect.contains(demandedTextRect)) {
					// fall back to use smallest useful font / even if text may be clipped
					font.setPointSize(fontSizeValue - 6);
					painter.setFont(font);
				}
			}
		}

		if (showLabel) {
			painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignHCenter, first->name);
		}

		n++;
	}

}

//This is a debugging method to print areas of rectangles to the console
void TreeMapPlot::areaReport()
{
	foreach(TreeMap *first, root->children) {
		QRect firstRect = first->rect;
		cout << "First level rectangle::: Width: " << firstRect.width() << " Height: " << firstRect.height() << " Area: " << firstRect.width() * firstRect.height() << " Value " << first->value << endl;

		foreach(TreeMap *second, first->children) {
			QRect secondRect = second->rect;
			cout << "Second level rectangle::: Width: " << secondRect.width() << " Height: " << secondRect.height() << " Area: " << secondRect.width() * secondRect.height() << " Value: " << second->value << endl;
		}

	}

}

//This function builds a linked list of leaf nodes from the display tree structure
void TreeMapPlot::buildLeafList(TreeMap * parent)
{
	if (parent->children.size() > 0) {
		for (int i = 0; i < parent->children.size(); i++) {
			if (parent->children[i] != nullptr) {
				buildLeafList(parent->children[i]);
			}

		}
	}
	else {
		leafNodes.push_back(parent);
	}

}

//This function rebuilds the layout
//It first rebuilds the display tree.  Then it rebuilds the layout on that display tree.
void TreeMapPlot::issueLayout() 
{
	buildTree();

	if (root) {

		switch (layoutMethod) {
		case 0:
		case 2:
			root->layout(QRect(9, 9, geometry().width() - 18, geometry().height() - 18), true);
			break;
		case 1:
			root->layout(QRect(9, 9, geometry().width() - 18, geometry().height() - 18), false);
			break;
		default:
			cerr << "Bad layout request with id " << layoutMethod << ".  This should not happen." << endl;
			break;
		}
	}

	repaint();

}

//This method receives and responds to events of interest
//Parameter e - the event being processed
bool TreeMapPlot::eventFilter(QObject *, QEvent *e)
{
	//Respond to mouse movement events (they highlight rectangles with a brighter overall color)
	if (e->type() == QEvent::MouseMove) {
		QPoint pos = static_cast<QMouseEvent*>(e)->pos();
		TreeMap *underMouse = NULL;

		// look at the bottom rung.
		foreach(TreeMap *first, leafNodes)
			if ((underMouse = first->findAt(pos)) != NULL)
				break;

		// if this one isn't the one that is
		// currently highlighted repaint to
		// highlight it!
		if (underMouse && highlight != underMouse) {
			highlight = underMouse;
			repaint();
			return true;
		}

	}
	else if (e->type() == QEvent::Leave) {
		highlight = NULL;
		repaint();
		return true;

	}
	//Respond to left mouse button clicks (they select rectangles)
	else if (e->type() == QEvent::MouseButtonPress) {

		QPoint pos = static_cast<QMouseEvent*>(e)->pos();
		Qt::MouseButton button = static_cast<QMouseEvent*>(e)->button();

		if (button == Qt::LeftButton) {
			TreeMap *underMouse = NULL;

			// look at the bottom rung.
			foreach(TreeMap *first, leafNodes)
				if ((underMouse = first->findAt(pos)) != NULL)
					break;
			
			// got one?
			if (underMouse && underMouse->nodeBiRef != nullptr) {

				underMouse->nodeBiRef->SetSelected(!underMouse->nodeBiRef->GetSelected());

				if (underMouse->nodeBiRef->GetGraphNode() != nullptr) {
					Widget::Node * nodePtr = underMouse->nodeBiRef->GetGraphNode();
					nodePtr->update();
				}

				repaint();

				return true;
			}
		}
	}
	//Respond to right mouse click events (they change the layout used)
	else if (e->type() == QEvent::ContextMenu) {
		cout << "Right Mouse Click" << endl;
		
		layoutMethod = (layoutMethod + 1) % 3;
		issueLayout();
		update();
		
	}
	

	return false;
}


