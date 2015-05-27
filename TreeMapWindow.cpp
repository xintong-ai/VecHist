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

#include "TreeMapWindow.h"
#include "TreeMapPlot.h"

#include <qscrollarea.h>
#include <QtGui>
#include <QString>
#include <QVBoxLayout>
#include <qformlayout.h>
#include <iostream>



TreeMapWindow::TreeMapWindow()
{
	// the plot
	mainLayout = new QVBoxLayout;  //Lines widgets up vertically
	ltmPlot = new TreeMapPlot(this);
	mainLayout->addWidget(ltmPlot);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

	//resize(sizeHint() * 10);

	// user clicked on a cell in the plot
	connect(ltmPlot, SIGNAL(clicked(QString, QString)), this, SLOT(cellClicked(QString, QString)));
    
}

TreeMapWindow::~TreeMapWindow()
{
}

void
TreeMapWindow::refreshPlot(NodeBi * root)
{
	ltmPlot->setData(root);
}

void
TreeMapWindow::cellClicked(QString f1, QString f2)
{
	//This appears to be code from Golden Cheetah for generating a popup with information from the click.  We will probably just strip this out after verifying it is useless to us.
	/*
	QStringList match;

	// create a list of activities in this cell
	int count = 0;
	foreach(RideItem *item, context->athlete->rideCache->rides()) {

		// honour the settings
		if (!settings.specification.pass(item)) continue;

		// text may either not exists, then "unknown" or just be "" but f1, f2 don't know ""
		QString x1 = item->getText(settings.field1, tr("(unknown)"));
		QString x2 = item->getText(settings.field2, tr("(unknown)"));
		if (x1 == "") x1 = tr("(unknown)");
		if (x2 == "") x2 = tr("(unknown)");

		// match !
		if (x1 == f1 && x2 == f2) {
			match << item->fileName;
			count++;
		}
	}

	// create a specification for ours
	Specification spec;
	spec.setDateRange(settings.specification.dateRange());
	FilterSet fs = settings.specification.filterSet();
	fs.addFilter(true, match);
	spec.setFilterSet(fs);

	// and the metric to display
	const RideMetricFactory &factory = RideMetricFactory::instance();
	const RideMetric *metric = factory.rideMetric(settings.symbol);

	ltmPopup->setData(spec, metric, QString(tr("%1 activities")).arg(count));
	popup->show();
	*/
	std::cout << "Clicked in the tree map window " << std::endl;
}

const double EPSILON2 = 1e-02;

#ifndef QT_NO_WHEELEVENT
void TreeMapWindow::wheelEvent(QWheelEvent *event)
{
	//cout << "Scroll wheel event " <<  << endl;

	double factor = 1;
	if (abs(event->delta()) < EPSILON2) {
		factor = 1;
	}
	else if (event->delta() > 0) {
		factor = event->delta() / 100.0;
	}
	else if (event->delta() < 0) {
		factor = 1 / (fabs(event->delta() / 100.0));
	}
	

	int width = this->width();
	int height = this->height();

	setFixedSize(width * factor, height*factor);

	QPoint point = event->pos();

	int x = point.x();
	int y = point.y();

	
	scrollArea->ensureVisible(x * factor, y * factor);
}
#endif

