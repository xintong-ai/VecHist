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

TreeMapPlot::TreeMapPlot(TreeMapWindow *parent)
	: QWidget(parent), parent(parent)
{
	root = new TreeMap;
	setMouseTracking(true);
	installEventFilter(this);
    
}

TreeMapPlot::~TreeMapPlot()
{
}

void TreeMapPlot::setData(NodeBi * rootBi)
{
	if (root != nullptr) {
		root->clear();
	}
	

	/*
	std::vector<QString> strings1;
	std::vector<QString> strings2;
	std::vector<QString> strings3;

	std::vector<double> values1;
	std::vector<double> values2;
	std::vector<double> values3;

	strings1.push_back("John");
	strings1.push_back("Jill");
	strings1.push_back("Sam");
	strings1.push_back("Walter");

	values1.push_back(0);
	values1.push_back(0);
	values1.push_back(0);
	values1.push_back(0);

	strings2.push_back("1");
	strings2.push_back("2");
	strings2.push_back("3");
	strings2.push_back("4");

	//Area differences test - I would expect item 3 to have an area 4 X as large as the other 3 entries
	//Interesting test case that fails in squarify layout: 50, 50, 200, 100.
	//Here we did not push any items to the 3rd level to do this test case (I commented out the 3rd level nesting insertions).
	values2.push_back(0);
	values2.push_back(0);
	values2.push_back(0);
	values2.push_back(0);

	strings3.push_back("a");
	strings3.push_back("b");
	strings3.push_back("c");
	strings3.push_back("d");

	values3.push_back(100);
	values3.push_back(100);
	values3.push_back(200);
	values3.push_back(100);

	for (int i = 0; i < strings1.size(); i++) {
		QString text1 = strings1[i];
		TreeMap *first = root->insert(text1, values1[i]);
		for (int j = 0; j < strings2.size(); j++) {
			QString text2 = strings2[j];
			TreeMap * second = first->insert(text2, values2[j]);
			for (int k = 0; k < strings3.size(); k++) {
				//3 level nesting test case -- we need to make the paint method work for this
				QString text3 = strings3[k];
				TreeMap * third = second->insert(text3, values3[i]);
			}

			
		}
	}
	*/
	
	parseBITree(rootBi);
		

	leafNodes.clear();
	buildLeafList(root);

	// layout and paint
	resizeEvent(NULL);
	repaint();
}

void TreeMapPlot::parseBITree(NodeBi * biNode)
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
			parseBITree(biNode->GetLeft(), root, 0);
		}
		if (biNode->GetRight() != nullptr) {
			parseBITree(biNode->GetRight(), root, 0);
		}
	}

}

void TreeMapPlot::parseBITree(NodeBi * biNode, TreeMap * treeMapNode, int currentDepth)
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
		parseBITree(biNode->GetLeft(), newNode, currentDepth + 1); 
	}
	if (biNode->GetRight() != nullptr && currentDepth < DEPTH_LIMIT) {
		parseBITree(biNode->GetRight(), newNode, currentDepth + 1);
	}


}

void TreeMapPlot::resizeEvent(QResizeEvent *)
{
	// layout the map
	//if (root) root->layout(QRect(9, 9, geometry().width() - 18, geometry().height() - 18));
	if (root) root->layout(QRect(9, 9, geometry().width() - 18, geometry().height() - 18));
}

void TreeMapPlot::paintEvent(QPaintEvent *)
{
	//areaReport();
	if (!root) return;

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
	painter.drawRect(root->rect.x() + 4,
		root->rect.y() + 4,
		root->rect.width() - 8,
		root->rect.height() - 8);

	// first level - rectangles, but not text yet
	pen.setWidth(5);
	pen.setColor(color);
	painter.setPen(pen);

	paintChildren(root, painter, brush, 0);




}

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


	int fontSizeValue = 0;

	switch (level) {
	case 0:
		textPen.setColor(Qt::darkMagenta);
		fontSizeValue = 18;
		font.setPointSize(fontSizeValue);
		break;
	case 1:
		textPen.setColor(Qt::magenta);
		fontSizeValue = 16;
		font.setPointSize(fontSizeValue);
		break;
	default:
		textPen.setColor(Qt::white);
		fontSizeValue = 14;
		font.setPointSize(fontSizeValue);
		break;
	}

	painter.setFont(font);

	int n = 1;
	QColor cHSV, cRGB;
	double factor = double(1) / double(parent->children.count());

	foreach(TreeMap *first, parent->children) {

		

		cHSV.setHsv((double)255 / (factor*n++), 255, 150);
		cRGB = cHSV.convertTo(QColor::Rgb);
		brush.setColor(cRGB);
		
		if (first == highlight)
			painter.setBrush(hbrush);
		else
			painter.setBrush(brush);
		
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

		//font.setPointSize(14);
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

		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignHCenter, first->name);
	}

}

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

bool TreeMapPlot::eventFilter(QObject *, QEvent *e)
{
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
	else if (e->type() == QEvent::ContextMenu) {
		cout << "Right Mouse Click" << endl;
		if (root) {
			root->useSquareLayout = !root->useSquareLayout;
			root->layout(QRect(9, 9, geometry().width() - 18, geometry().height() - 18));
			update();
		}
	}
	

	return false;
}


