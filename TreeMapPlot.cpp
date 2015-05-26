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

void
TreeMapPlot::setData()
{
	root->clear();

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
	values2.push_back(100);
	values2.push_back(100);
	values2.push_back(200);
	values2.push_back(100);

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
				TreeMap * third = second->insert(text3, values3[k]);
			}

			
		}
	}

	// layout and paint
	resizeEvent(NULL);
	repaint();
}

void
TreeMapPlot::resizeEvent(QResizeEvent *)
{
	// layout the map
	if (root) root->layout(QRect(9, 9, geometry().width() - 18, geometry().height() - 18));
}

void
TreeMapPlot::paintEvent(QPaintEvent *)
{
	//areaReport();

	/*
	if (!root) return;

	// labels
	QFont font;

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

	int n = 1;
	QColor cHSV, cRGB;
	double factor = double(1) / double(root->children.count());

	foreach(TreeMap *first, root->children) {

		cHSV.setHsv((double)255 / (factor*n++), 255, 150);
		cRGB = cHSV.convertTo(QColor::Rgb);
		brush.setColor(cRGB);
		painter.setBrush(brush);
		painter.drawRect(first->rect);
	}

	// second level - overlay rectangles - with text
	QPen textPen(Qt::white);
	font.setPointSize(8);
	painter.setFont(font);

	// overlay colors
	color = Qt::darkGray;
	color.setAlpha(127);
	brush.setColor(color);
	QColor hcolor(Qt::lightGray);
	hcolor.setAlpha(127);
	QBrush hbrush(hcolor);
	QRect textRect;
	QRect demandedTextRect;
	foreach(TreeMap *first, root->children) {
		foreach(TreeMap *second, first->children) {
			if (second == highlight) painter.setBrush(hbrush);
			else painter.setBrush(brush);

			//////////////rectangles////////////
			painter.setPen(Qt::NoPen);
			painter.drawRect(second->rect.x() + 2,
				second->rect.y() + 2,
				second->rect.width() - 4,
				second->rect.height() - 4);

			//////////////text///////////////////
			textRect.setRect(second->rect.x() + 2, second->rect.y() + 2, second->rect.width() - 4, second->rect.height() - 4);
			// determine font size based on size of window and length of text / try to apply 3 different font sizes - then fall back to "8"
			font.setPointSize(14);
			painter.setFont(font);
			painter.setPen(textPen);
			demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
			if (!textRect.contains(demandedTextRect)) {
				font.setPointSize(12);
				painter.setFont(font);
				demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
				if (!textRect.contains(demandedTextRect)) {
					font.setPointSize(10);
					painter.setFont(font);
					demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
					if (!textRect.contains(demandedTextRect)) {
						// fall back to use smallest useful font / even if text may be clipped
						font.setPointSize(8);
						painter.setFont(font);
					}
				}
			}
			painter.drawText(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
		}
	}

	// paint the text for level 1 now - to not overlap it with gray/alpha coming from level 2 drawing overlay
	textPen.setColor(Qt::black);
	font.setPointSize(18);
	painter.setFont(font);
	painter.setPen(textPen);
	foreach(TreeMap *first, root->children) {
		painter.drawText(first->rect,
			Qt::AlignVCenter | Qt::AlignHCenter,
			first->name);
	}
	*/
	

	if (!root) return;

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

	


//////////////////////////////////////
// second level - overlay rectangles - with text

// overlay colors
//color = Qt::darkGray;
//color.setAlpha(127);
//brush.setColor(color);
//QColor hcolor(Qt::lightGray);
//hcolor.setAlpha(127);
//QBrush hbrush(hcolor);


#ifdef JUNK
QRect textRect;
QRect demandedTextRect;
foreach(TreeMap *first, root->children) {
	foreach(TreeMap *second, first->children) {
		if (second == highlight) painter.setBrush(hbrush);
		else painter.setBrush(brush);

		//////////////rectangles////////////
		painter.setPen(Qt::NoPen);
		painter.drawRect(second->rect.x() + 2,
			second->rect.y() + 2,
			second->rect.width() - 4,
			second->rect.height() - 4);

		//////////////text///////////////////
		textRect.setRect(second->rect.x() + 2, second->rect.y() + 2, second->rect.width() - 4, second->rect.height() - 4);
		// determine font size based on size of window and length of text / try to apply 3 different font sizes - then fall back to "8"
		font.setPointSize(14);
		painter.setFont(font);
		painter.setPen(textPen);
		demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
		if (!textRect.contains(demandedTextRect)) {
			font.setPointSize(12);
			painter.setFont(font);
			demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
			if (!textRect.contains(demandedTextRect)) {
				font.setPointSize(10);
				painter.setFont(font);
				demandedTextRect = painter.boundingRect(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
				if (!textRect.contains(demandedTextRect)) {
					// fall back to use smallest useful font / even if text may be clipped
					font.setPointSize(8);
					painter.setFont(font);
				}
			}
		}
		painter.drawText(textRect, Qt::AlignTop | Qt::AlignLeft, second->name);
	}
}

// paint the text for level 1 now - to not overlap it with gray/alpha coming from level 2 drawing overlay
textPen.setColor(Qt::black);
font.setPointSize(18);
painter.setFont(font);
painter.setPen(textPen);
foreach(TreeMap *first, root->children) {
	painter.drawText(first->rect,
		Qt::AlignVCenter | Qt::AlignHCenter,
		first->name);
}
#endif
}

void TreeMapPlot::paintChildren(TreeMap * parent, QPainter & painter, QBrush & brush, int level)
{
	QFont font;
	QPen textPen;
	QColor hcolor(Qt::lightGray);
	hcolor.setAlpha(127);
	QBrush hbrush(hcolor);

	int fontSizeValue = 0;

	switch (level) {
	case 0:
		textPen.setColor(Qt::magenta);
		fontSizeValue = 18;
		font.setPointSize(fontSizeValue);
		break;
	case 1:
		textPen.setColor(Qt::yellow);
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
		
		painter.setBrush(brush);
		painter.setPen(textPen);
		painter.drawRect(first->rect);

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

bool
TreeMapPlot::eventFilter(QObject *, QEvent *e)
{

	if (e->type() == QEvent::MouseMove) {
		QPoint pos = static_cast<QMouseEvent*>(e)->pos();
		TreeMap *underMouse = NULL;

		// look at the bottom rung.
		foreach(TreeMap *first, root->children)
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
			foreach(TreeMap *first, root->children)
				if ((underMouse = first->findAt(pos)) != NULL)
					break;

			// got one?
			if (underMouse) {
				emit clicked(underMouse->parent->name, underMouse->name);
			}
		}
	}

	return false;
}
