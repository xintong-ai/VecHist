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

TreeMapPlot::TreeMapPlot(TreeMapWindow *parent)
	: QWidget(parent), parent(parent)
{
	root = new TreeMap;

	myLabel.setText("Test Successful");
	
	QVBoxLayout * mainLayout = new QVBoxLayout;  //Lines widgets up vertically
	
	mainLayout->addWidget(&myLabel);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	//setLayout(mainLayout);
    
}

TreeMapPlot::~TreeMapPlot()
{
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
	QPainter painter(this);

	QColor color = QColor(0, 0, 255, 255);
	QPen pen(color);
	pen.setWidth(10); // root
	QBrush brush(color);
	painter.setBrush(brush);
	painter.setPen(pen);

	painter.drawRect(root->rect.x() + 4,
		root->rect.y() + 4,
		root->rect.width() - 8,
		root->rect.height() - 8);
}

